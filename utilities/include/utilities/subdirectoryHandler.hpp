#pragma once
#include "auxiliary.inl"
#include "tiffio.h"
#include "utilities/exception.hpp"
#include <QDebug>
#include <QImage>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utilities/annotations.hpp>
#include <utilities/archiver.hpp>
#include <utility>
#include <vector>

namespace fs = std::experimental::filesystem;
namespace tsr = Tesseract;
using json = nlohmann::json;

struct SubdirectoryHandler
{
  explicit SubdirectoryHandler(std::string const& subDir, size_t ctr) try
    : snapshotDirPath_ {
    subDir
  }
  , id_{ ctr }, serializationPathFar_{ subDir + "/radarIntensityFar.json" },
    serializationPathNear_{ subDir + "/radarIntensityNear.json" }
  {
    assert(fs::exists(snapshotDirPath_) && fs::is_directory(snapshotDirPath_));
    readFileNamesOfDirectory();
    std::sort(std::begin(admaFileNames_), std::end(admaFileNames_));
    std::sort(std::begin(tiffFileNames_), std::end(tiffFileNames_));
    ExtractFromTIFF();
    ExtractFromADMA();
    ExtractFromPCD();
    ExtractFromJSON();
  }
  catch (const Exception& ex) { std::cerr << ex.what() << '\n'; }

  void readFileNamesOfDirectory();
  void ExtractFromTIFF();
  void ExtractFromPCD();
  void ExtractFromADMA();
  void ExtractFromJSON();

  void archive(const RadarArchiver&);
  void wipeArchive(tsr::ARS300);
  void serializeJSON() const;

  size_t id_;
  fs::path snapshotDirPath_;
  std::vector<std::string> tiffFileNames_;
  std::vector<std::string> pcdFileNames_;
  std::vector<std::string> admaFileNames_;
  std::vector<std::string> jsonFileNames_;
  // ADMA
  using EgoPositions = std::vector<tsr::Position2D>;
  EgoPositions egoPositions_;
  // Radar
  tsr::ARS300 radarMode_;
  using RadarDataFar = std::unordered_map<std::string, std::vector<uint16_t>>;
  RadarDataFar radarDataFar;
  std::unordered_map<std::string, QImage> radarImagesFar;

  using RadarDataNear = std::unordered_map<std::string, std::vector<uint16_t>>;
  RadarDataNear radarDataNear;
  std::unordered_map<std::string, QImage> radarImagesNear;
  // Lidar
  using LidarPointClouds = // Lidarname -> laserpoints -> pointcoords in float
    std::unordered_map<std::string, std::vector<tsr::Point3D>>;
  LidarPointClouds pointClouds;
  json lidarAnnootations_;
  // Camera
  std::unordered_map<std::string, QImage> camImages;
  // Labels
  const json& annotations(tsr::ARS300) const;
  json radarAnnotationsFar_;
  json radarAnnotationsNear_;
  std::string serializationPathFar_, serializationPathNear_;
};
