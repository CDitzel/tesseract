#pragma once

#include <models/admaModel.hpp>
#include <models/imageModel.hpp>
#include <models/lidarModel.hpp>
#include <models/radarModel.hpp>
#include <models/stereoModel.hpp>
#include <utilities/subdirectoryHandler.hpp>
#include <utilities/typeErasure.hpp>
#include <vector>

struct SensorModels
{
  explicit SensorModels(const SubdirectoryHandler& snapshot)
    : admaModel_{ snapshot.egoPositions_ }
    , radarModel_{ admaModel_, snapshot.radarDataFar, snapshot.radarDataNear }
    , lidarModel_{ admaModel_, snapshot.pointClouds }
    , stereoModel_{ admaModel_, snapshot.pointClouds }
  {
  }
  AdmaModel admaModel_;
  RadarModel radarModel_;
  LidarModel lidarModel_;
  ImageModel imageModel_;
  StereoModel stereoModel_;

  const RadarModel& radar() const { return radarModel_; }
};
