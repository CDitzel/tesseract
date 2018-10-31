#include <chrono>
#include <gui/currentState.hpp>
#include <typeinfo>

QString
CurrentState::update(const SubdirectoryHandler& snapshot)
{
  auto start = std::chrono::system_clock::now();
  // lazy evaluation
  auto [iter, success] { recordings_.try_emplace(snapshot.id_, snapshot) };

  Annotator::Instance().resetLabelCounter();
  emit updateAppearance(iter->first);
  constructHistory(snapshot);

  auto stop{ std::chrono::system_clock::now() };
  std::chrono::duration<double, std::milli> time{ stop - start };

  QString diagnosis{ QString::number(iter->first + 1) + " calculated in " +
                     QString::number(time.count()) + " ms " };
  return success ? QString{ "Novel Model " + diagnosis }
                 : QString{ "Recovered Model " + diagnosis };
}

void
CurrentState::drawSelection()
{

  if (toggleAnnotation_ == Sensor::RADAR) {
    auto numberOfRadarVoxel{ tsr::count(current(radarIllustration_)) };
    for (auto id{ 0 }; id < numberOfRadarVoxel; ++id) {
      glPushName(id);
      tsr::process(current(radarIllustration_).at(id));
      glPopName();
    }
  } else if (toggleAnnotation_ == Sensor::LIDAR) {
    auto numberOfLidarPoints{ tsr::count(current(lidarIllustration_)) };
    for (auto id{ 0 }; id < numberOfLidarPoints; ++id) {
      glPushName(id);
      tsr::process(current(lidarIllustration_).at(id));
      glPopName();
    }
  }
}

void
CurrentState::draw()
{
  tsr::process(current(radarIllustration_));
  tsr::process(current(lidarIllustration_));
}

bool
CurrentState::makeHistory(const std::set<uint16_t>& _selection, Label _label)
{
  tsr::wipe(radarStash_, lidarStash_);
  tsr::obliterate(radarLabelStash_, lidarLabelStash_);
  auto _commitLabel = std::stringstream{};
  _commitLabel << _label;
  if (toggleAnnotation_ == Sensor::RADAR) {
    auto _radarArchiver = RadarArchiver{ rangeMode_ };
    _radarArchiver["label"] = _commitLabel.str();
    _radarArchiver["polygon"] = modifyRadar(_selection, _label);
    radarLabels_.emplace_back(_radarArchiver);
    _commitLabel << Annotator::Instance().incrementLabelCounter(_label)
                 << "Radar";
  } else if (toggleAnnotation_ == Sensor::LIDAR) {
    auto _lidarArchiver = LidarArchiver{};
    _lidarArchiver["label"] = _commitLabel.str();
    _lidarArchiver["collection"] = modifyLidar(_selection, _label);
    lidarLabels_.emplace_back(_lidarArchiver);
    _commitLabel << Annotator::Instance().incrementLabelCounter(_label)
                 << "Lidar";
  }
  emit updateHistoryStack(_commitLabel.str());
  return true;
}

json
CurrentState::modifyRadar(const std::set<uint16_t>& _selection, Label _label)
{
  tsr::commit(radarIllustration_, _label);
  const auto& _pixelator{ tsr::index_to_pixel(NUMBER_BEAMS) };
  auto _polygonArray = json{};
  for (const auto& _index : _selection) {
    tsr::current(radarIllustration_).at(_index) =
      RadarVoxel{ recordings_.at(cursor).radar().mesh(rangeMode_).at(_index),
                  _label };
    const auto & [ _x, _y ] = _pixelator(_index);
    _polygonArray.push_back({ _x, _y });
  }
  return _polygonArray;
}

json
CurrentState::modifyLidar(const std::set<uint16_t>& _selection, Label _label)
{
  tsr::commit(lidarIllustration_, _label);
  auto _cloudArray = json{};
  for (const auto& _index : _selection) {
    auto _point =
      recordings_.at(cursor).lidarModel_.lidarTransformed_.at(_index);
    tsr::current(lidarIllustration_).at(_index) = LidarPoint{ _point, _label };
    _cloudArray.push_back({ _point.x_, _point.y_, _point.z_ });
  };
  return _cloudArray;
}

void
CurrentState::undoHistory()
{
  if (toggleAnnotation_ == Sensor::RADAR) {
    if (auto _radarLayer{ tsr::revoke(radarIllustration_) }; _radarLayer) {
      tsr::store(radarStash_, *_radarLayer);
      radarLabelStash_.emplace_back(radarLabels_.back());
      radarLabels_.pop_back();
      emit removeItemFromHistoryStack();
    }
  } else if (toggleAnnotation_ == Sensor::LIDAR) {
    if (auto _lidarLayer{ tsr::revoke(lidarIllustration_) }; _lidarLayer) {
      tsr::store(lidarStash_, *_lidarLayer);
      lidarLabelStash_.emplace_back(lidarLabels_.back());
      lidarLabels_.pop_back();
      emit removeItemFromHistoryStack();
    }
  }
}

void
CurrentState::redoHistory()
{
  if (toggleAnnotation_ == Sensor::RADAR) {
    if (auto _radarLayer{ tsr::revoke(radarStash_) }; _radarLayer) {
      tsr::store(radarIllustration_, *_radarLayer);
      radarLabels_.emplace_back(radarLabelStash_.back());
      radarLabelStash_.pop_back();
      emit restoreItemInHistoryStack(labelToString(_radarLayer->first));
    }
  } else if (toggleAnnotation_ == Sensor::LIDAR) {
    if (auto _lidarLayer{ tsr::revoke(lidarStash_) }; _lidarLayer) {
      tsr::store(lidarIllustration_, *_lidarLayer);
      lidarLabels_.emplace_back(lidarLabelStash_.back());
      lidarLabelStash_.pop_back();
      emit restoreItemInHistoryStack(labelToString(_lidarLayer->first));
    }
  }
}

void
CurrentState::archiveHistory()
{
  DataHandler::Instance().wipeArchive(rangeMode_);
  //  tsr::process(radarLabel_);
}

void
CurrentState::startHistory(const SensorModels& _sensorModels)
{
  tsr::wipe(radarIllustration_, radarStash_, lidarIllustration_, lidarStash_);
  tsr::obliterate(radarLabels_, radarLabelStash_, lidarLabels_,
                  lidarLabelStash_);
  for (const auto& _shell : _sensorModels.radar().mesh(rangeMode_))
    tsr::current(radarIllustration_).emplace_back(RadarVoxel{ _shell });
  for (const auto& _point : _sensorModels.lidarModel_.lidarTransformed_)
    tsr::current(lidarIllustration_).emplace_back(LidarPoint{ _point });
}

void
CurrentState::constructHistory(const SubdirectoryHandler& snapshot)
{
  startHistory(recordings_.at(cursor));

  //  if (std::size(snapshot.annotations(mode_)) < 3)
  //    return;
  if (auto& objects{ snapshot.annotations(rangeMode_)["objects"] };
      !objects.empty()) {
    recoverHistory(objects);
  }
}

void
CurrentState::recoverHistory(const json& objects)
{
  const auto& _indexator{ tsr::pixel_to_index(NUMBER_BEAMS) };
  for (const auto& object : objects) {
    radarLabels_.emplace_back(RadarArchiver{ rangeMode_, object });
    const Label _label{ stringToLabel(object["label"]) };

    auto _selection = std::set<int>{};
    for (auto const& pixel : object["polygon"]) {
      auto _index = _indexator(tsr::Pixel{ pixel[0], pixel[1] });
      _selection.emplace(_index);
    }

    //    tsr::commit(radarIllustration_, _label);
    //    tsr::current(radarIllustration_)[_index] =
    //      RadarVoxel{ recordings_.at(cursor).radar().mesh(mode_)[_index],
    //      _label };
    //  }

    std::stringstream _commitLabel{};
    _commitLabel << _label
                 << Annotator::Instance().incrementLabelCounter(_label);
    emit updateHistoryStack(_commitLabel.str());
  }
}

void /* Signal*/
  CurrentState::onCommandEntered(std::string cmd)
{
  if (cmd == "fwd")
    ++cursor;
  else if (cmd == "bwd" && cursor > 0)
    --cursor;
  archiveHistory();
  emit command(std::to_string(cursor));
}

void
CurrentState::onSwitchRange()
{
  archiveHistory();
  rangeMode_ =
    rangeMode_ == tsr::ARS300::FRS ? tsr::ARS300::NRS : tsr::ARS300::FRS;
  emit command(std::to_string(cursor));
}

void
CurrentState::onToggleSensorAnnotation()
{
  toggleAnnotation_ =
    toggleAnnotation_ == Sensor::RADAR ? Sensor::LIDAR : Sensor::RADAR;
  std::cerr << "SENSOR " << static_cast<int>(toggleAnnotation_) << std::endl;
}
