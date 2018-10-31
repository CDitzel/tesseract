#pragma once

#include "auxiliary.inl"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
namespace tsr = Tesseract;

/*!
 * \brief The RadarArchiver struct
 */
struct RadarArchiver
{
  tsr::ARS300 mode_;
  json object_;

public:
  RadarArchiver(tsr::ARS300 mode)
    : mode_{ mode }
  {
  }

  RadarArchiver(tsr::ARS300 mode, json j)
    : mode_{ mode }
    , object_(j) // pay fucking attention to lohmanns bracket rules for
                 // discriminating between array and object initialization
  {
  }

  auto operator[](std::string s) -> decltype(object_[s]) { return object_[s]; }
};

/*!
 * \brief The LidarArchiver struct
 */
struct LidarArchiver
{
  json object_;

public:
  LidarArchiver() {}

  auto operator[](std::string s) -> decltype(object_[s]) { return object_[s]; }
};
