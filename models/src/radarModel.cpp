#include "models/radarModel.hpp"
#include <algorithm>

namespace tsr = Tesseract;

RadarModel::RadarModel(const AdmaModel& admaModel,
                       const SubdirectoryHandler::RadarDataFar& radarDataFar,
                       const SubdirectoryHandler::RadarDataNear& radarDataNear)
{
  std::cout << "RADARMODEL CONSTRUCTED" << std::endl;

  float speed = 4.f;

  if (speed < VELOCITY_THRESHOLD) {
    resoFar_ = 0.3f;
    resoNear_ = 0.25f;
    rangeFar_ = RangeFar::SLOW;
    rangeNear_ = RangeNear::SLOW;
  } else {
    resoFar_ = 1.0f;
    resoNear_ = 0.6f;
    rangeFar_ = RangeFar::FAST;
    rangeNear_ = RangeNear::FAST;
  }

  //  std::cerr << "PRECISION " << resoFar_ << std::endl;
  //  std::cerr << "PRECISION " << std::setprecision(10) << resoFar_ <<
  //  std::endl;

  // prevent pOtential reallocations
  scaledIntensitiesFar_.reserve(NUMBER_BEAMS * NUMBER_RADAR_CELLS_FAR);
  scaledIntensitiesNear_.reserve(NUMBER_BEAMS * NUMBER_RADAR_CELLS_NEAR);
  meshFar_.reserve(NUMBER_BEAMS * NUMBER_RADAR_CELLS_FAR);
  meshNear_.reserve(NUMBER_BEAMS * NUMBER_RADAR_CELLS_FAR);

  const auto [minPeakFar, maxPeakFar] {
    std::minmax_element(std::cbegin(radarDataFar.at("radar_far_intensity")),
                        std::cend(radarDataFar.at("radar_far_intensity")))
  };

  std::transform(std::cbegin(radarDataFar.at("radar_far_intensity")),
                 std::cend(radarDataFar.at("radar_far_intensity")),
                 std::back_inserter(scaledIntensitiesFar_),
                 tsr::scaleValue(*minPeakFar, *maxPeakFar));

  const auto [minPeakNear, maxPeakNear] {
    std::minmax_element(std::cbegin(radarDataNear.at("radar_near_intensity")),
                        std::cend(radarDataNear.at("radar_near_intensity")))
  };

  std::transform(std::cbegin(radarDataNear.at("radar_near_intensity")),
                 std::cend(radarDataNear.at("radar_near_intensity")),
                 std::back_inserter(scaledIntensitiesNear_),
                 tsr::scaleValue(*minPeakNear, *maxPeakNear));

  calculateRadarMesh();
}

const Mesh&
RadarModel::mesh(tsr::ARS300 _mode) const
{
  if (_mode == tsr::ARS300::FRS)
    return meshFar_;
  if (_mode == tsr::ARS300::NRS)
    return meshNear_;
}

void
RadarModel::calculateRadarMesh()
{
  uint16_t voxelIndex{ 0 };
  //  auto _stepsize{};
  for (auto r{ 0 }; r < NUMBER_RADAR_CELLS_FAR; ++r) {
    for (auto az{ 0 }; az < NUMBER_BEAMS; ++az) {

      Vertex v1{
        (r * resoFar_ + shrinkR) * cos(aziFRS[az] - halfAngleFar + shrinkAzi),
        (r * resoFar_ + shrinkR) * sin(aziFRS[az] - halfAngleFar + shrinkAzi),
        0.f
      };
      Vertex v2{ ((r + 1) * resoFar_ - shrinkR) *
                   cos(aziFRS[az] - halfAngleFar + shrinkAzi),
                 ((r + 1) * resoFar_ - shrinkR) *
                   sin(aziFRS[az] - halfAngleFar + shrinkAzi),
                 0.f };
      Vertex v3{ ((r + 1) * resoFar_ - shrinkR) *
                   cos(aziFRS[az] + halfAngleFar - shrinkAzi),
                 ((r + 1) * resoFar_ - shrinkR) *
                   sin(aziFRS[az] + halfAngleFar - shrinkAzi),
                 0.f };
      Vertex v4{
        (r * resoFar_ + shrinkR) * cos(aziFRS[az] + halfAngleFar - shrinkAzi),
        (r * resoFar_ + shrinkR) * sin(aziFRS[az] + halfAngleFar - shrinkAzi),
        0.f
      };
      Vertex v5{ v1.x_, v1.y_, scaledIntensitiesFar_[voxelIndex] };
      Vertex v6{ v2.x_, v2.y_, scaledIntensitiesFar_[voxelIndex] };
      Vertex v7{ v3.x_, v3.y_, scaledIntensitiesFar_[voxelIndex] };
      Vertex v8{ v4.x_, v4.y_, scaledIntensitiesFar_[voxelIndex] };

      meshFar_.emplace_back(Shell{ v1, v2, v3, v4, v5, v6, v7, v8 });

      if (r < NUMBER_RADAR_CELLS_NEAR) {

        v1 = { (r * resoNear_ + shrinkR) *
                 cos(aziNRS[az] - halfAngleNear + shrinkAzi),
               (r * resoNear_ + shrinkR) *
                 sin(aziNRS[az] - halfAngleNear + shrinkAzi),
               0.f };
        v2 = { ((r + 1) * resoNear_ - shrinkR) *
                 cos(aziNRS[az] - halfAngleNear + shrinkAzi),
               ((r + 1) * resoNear_ - shrinkR) *
                 sin(aziNRS[az] - halfAngleNear + shrinkAzi),
               0.f };
        v3 = { ((r + 1) * resoNear_ - shrinkR) *
                 cos(aziNRS[az] + halfAngleNear - shrinkAzi),
               ((r + 1) * resoNear_ - shrinkR) *
                 sin(aziNRS[az] + halfAngleNear - shrinkAzi),
               0.f };
        v4 = { (r * resoNear_ + shrinkR) *
                 cos(aziNRS[az] + halfAngleNear - shrinkAzi),
               (r * resoNear_ + shrinkR) *
                 sin(aziNRS[az] + halfAngleNear - shrinkAzi),
               0.f };
        v5 = { v1.x_, v1.y_, scaledIntensitiesNear_[voxelIndex] };
        v6 = { v2.x_, v2.y_, scaledIntensitiesNear_[voxelIndex] };
        v7 = { v3.x_, v3.y_, scaledIntensitiesNear_[voxelIndex] };
        v8 = { v4.x_, v4.y_, scaledIntensitiesNear_[voxelIndex] };

        meshNear_.emplace_back(Shell{ v1, v2, v3, v4, v5, v6, v7, v8 });
      }
      ++voxelIndex;
    }
  }
}
