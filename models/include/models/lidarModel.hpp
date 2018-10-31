#pragma once
#include "models/admaModel.hpp"
#include "utilities/auxiliary.inl"
#include <iostream>
#include <unordered_map>
#include <vector>

namespace tsr = Tesseract;

struct LidarModel
{
  LidarModel(const AdmaModel&, const SubdirectoryHandler::LidarPointClouds&);

  // TODO: maybe make reference instead of copy?
  //  std::unordered_map<std::string, std::vector<Point3D>> lidarPointClouds_;
  //  SubdirectoryHandler::LidarPointClouds const& lidarPointClouds_;

  std::vector<tsr::Point3D> lidarTransformed_;
  //  std::vector<Tesseract::Point3D> ibeoTransformed_;
  //  std::vector<Tesseract::Point3D> veloFrontTransformed_;
  //  std::vector<Tesseract::Point3D> veloLeftTransformed_;
  //  std::vector<Tesseract::Point3D> veloRearTransformed_;
  //  std::vector<Tesseract::Point3D> veloRightTransformed_;
};
