#pragma once
#include "models/admaModel.hpp"
#include "utilities/dataHandler.hpp"
#include <array>
#include <string>

constexpr uint8_t NUMBER_BEAMS{ 17 };
constexpr uint8_t NUMBER_RADAR_CELLS_FAR{ 200 };
constexpr uint8_t NUMBER_RADAR_CELLS_NEAR{ 100 };

constexpr float VELOCITY_THRESHOLD{ 5.f };
constexpr float halfAngleFar{ 0.00872664625997165f };
constexpr float halfAngleNear{ 0.0305432677268982f };
// constexpr float shrinkR{ 0.1f };
// constexpr float shrinkAzi{ 0.0005f };
constexpr float shrinkR{ 0.005f };
constexpr float shrinkAzi{ 0.001f };

enum class RangeFar : uint8_t
{
  SLOW = 60,
  FAST = 200
};

enum class RangeNear : uint8_t
{
  SLOW = 25,
  FAST = 60
};

constexpr std::array<float, 17> aziFRS{
  8 * M_PI / 180.,  7 * M_PI / 180.,  6 * M_PI / 180.,  5 * M_PI / 180.,
  4 * M_PI / 180.,  3 * M_PI / 180.,  2 * M_PI / 180.,  1 * M_PI / 180.,
  0 * M_PI / 180.,  -1 * M_PI / 180., -2 * M_PI / 180., -3 * M_PI / 180.,
  -4 * M_PI / 180., -5 * M_PI / 180., -6 * M_PI / 180., -7 * M_PI / 180.,
  -8 * M_PI / 180.
};

constexpr std::array<float, 17> aziNRS = {
  30.00 * M_PI / 180.,  26.25 * M_PI / 180.,  22.50 * M_PI / 180.,
  18.75 * M_PI / 180.,  15.00 * M_PI / 180.,  11.25 * M_PI / 180.,
  7.50 * M_PI / 180.,   3.75 * M_PI / 180.,   0 * M_PI / 180.,
  -3.75 * M_PI / 180.,  -7.50 * M_PI / 180.,  -11.25 * M_PI / 180.,
  -15.00 * M_PI / 180., -18.75 * M_PI / 180., -22.50 * M_PI / 180.,
  -26.50 * M_PI / 180., -30.00 * M_PI / 180.
};

struct Vertex
{
  float x_, y_, z_;
};

struct Shell
{
  using Vertices = std::array<Vertex, 8>;
  Vertices vertices_;
};

// Likely to be faster than any map implementation
using Mesh = std::vector<Shell>;
using PeaksFar = std::vector<float>;
using PeaksNear = std::vector<float>;

struct RadarModel
{
  RadarModel(const AdmaModel&, const SubdirectoryHandler::RadarDataFar&,
             const SubdirectoryHandler::RadarDataNear&);
  ~RadarModel() { std::cout << "RADARMODEL DESTRUCTED" << std::endl; };

  Mesh meshFar_;
  PeaksFar scaledIntensitiesFar_;
  Mesh meshNear_;
  PeaksNear scaledIntensitiesNear_;

  RangeFar rangeFar_;
  RangeNear rangeNear_;

  float resoFar_;
  float resoNear_;

  const Mesh& mesh(tsr::ARS300) const;

  void calculateRadarMesh();
};
