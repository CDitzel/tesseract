#pragma once
#include "models/sensorModels.hpp"
#include "utilities/annotations.hpp"
#include "utilities/auxiliary.inl"
#include "utilities/dataHandler.hpp"
#include "utilities/typeErasure.hpp"
#include <QImage>
#include <QObject>
#include <QString>
#include <array>
#include <gui/lidarPoint.hpp>
#include <gui/radarVoxel.hpp>
#include <iterator>
#include <tuple>
#include <unordered_set>

using json = nlohmann::json;
namespace tsr = Tesseract;

static void
process(const RadarArchiver& archiver)
{
  DataHandler::Instance().archive(archiver);
}

static void
process(const LidarArchiver& archiver)
{
  //  DataHandler::Instance().archive(archiver);
}

class CurrentState : public QObject
{
  Q_OBJECT
  using Recordings = std::unordered_map<size_t, SensorModels>;

public:
  explicit CurrentState(QObject* parent = nullptr)
    : QObject{ parent } {};
  QString update(const SubdirectoryHandler&);

  void draw();
  void drawSelection();

  bool makeHistory(const std::set<uint16_t>& selection, Label label);
  void undoHistory();
  void redoHistory();
  void archiveHistory();

private:
  size_t cursor{ 0 };
  Recordings recordings_{};
  //  tsr::progress_t FRS_, NRS_, stashFRS_, stashNRS_;
  tsr::progress_t radarIllustration_, radarStash_;
  tsr::progress_t lidarIllustration_, lidarStash_;
  //  tsr::progress_t lidarIllustration_;
  //  tsr::progress_t stereoIllustration_;
  tsr::collection_t radarLabels_, radarLabelStash_;
  tsr::collection_t lidarLabels_, lidarLabelStash_;
  //  tsr::history_t lidarHistory_{ 1 };
  //  RadarModel* radarModel_{ nullptr };
  //  Mesh* mesh_{ nullptr };
  //  LidarModel* lidarModel_{ nullptr };

  json modifyRadar(std::set<uint16_t> const&, Label);
  json modifyLidar(std::set<uint16_t> const&, Label);
  tsr::ARS300 rangeMode_{ tsr::ARS300::NRS };

  enum class Sensor
  {
    RADAR,
    LIDAR
  } toggleAnnotation_;

  void startHistory(const SensorModels&);
  void constructHistory(const SubdirectoryHandler&);
  void recoverHistory(const json&);

public slots:
  void onCommandEntered(std::string cmd);
  void onSwitchRange();
  void onToggleSensorAnnotation();

signals:
  void removeItemFromHistoryStack();
  void restoreItemInHistoryStack(std::string);
  void updateHistoryStack(std::string);
  void command(std::string);
  void updateAppearance(size_t);
};
