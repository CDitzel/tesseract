#pragma once
#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
//#include <utilities/dataHandler.hpp>
//#include <utilities/subdirectoryHandler.hpp>

enum class Label : uint8_t
{
  Unlabeled,
  Sky,
  Car,
  Truck,
  Bus,
  Motorcycle,
  Pedestrian,
  Bicyclist,
  TrafficSign,
  TrafficLight,
  Road,
  RoadMarking,
  Sidewalk,
  Building,
  Vegetation,
  Pole,
  Animals,
  LostCargo,
};

Label stringToLabel(const std::string&);
std::string labelToString(Label);

// sRGB luminance(Y) values
const double rY{ 0.212655 };
const double gY{ 0.715158 };
const double bY{ 0.072187 };

// Inverse of sRGB "gamma" function. (approx 2.2)
static double
inv_gam_sRGB(int ic)
{
  double c = ic / 255.0;
  if (c <= 0.04045)
    return c / 12.92;
  else
    return std::pow(((c + 0.055) / (1.055)), 2.4);
}

// sRGB "gamma" function (approx 2.2)
static int
gam_sRGB(double v)
{
  if (v <= 0.0031308)
    v *= 12.92;
  else
    v = 1.055 * std::pow(v, 1.0 / 2.4) - 0.055;
  return static_cast<int>(v * 255 + 0.5);
}

// GRAY VALUE ("brightness")
static int
gray(int r, int g, int b)
{
  return gam_sRGB(rY * inv_gam_sRGB(r) + gY * inv_gam_sRGB(g) +
                  bY * inv_gam_sRGB(b));
}

std::ostream& operator<<(std::ostream& os, Label label);

/*!
 * \brief The Annotator class
 */
class Annotator
{
public:
  static Annotator& Instance()
  {
    static Annotator instance{};
    return instance;
  };

  struct Color
  {
    float r_, g_, b_, alpha{ 1.f };
  };

  // fuck the ordering, i.e. access in O(1)
  using LabelKeys = std::unordered_map<unsigned, Label>;
  using LabelColors = std::unordered_map<Label, Color>;
  using LabelCounter = std::unordered_map<Label, size_t>;

  LabelKeys labelKeys_;
  LabelColors labelColors_;
  LabelCounter labelCounter_;

  const LabelKeys& labelKeys() const { return labelKeys_; }
  const LabelColors& labelColors() const { return labelColors_; }
  LabelCounter& labelCounter() { return labelCounter_; }

  void setShortcut(unsigned key, Label label)
  {
    labelKeys_.insert_or_assign(key, label);
  }

  void setLabelColor(Label label, Color color)
  {
    labelColors_.insert_or_assign(label, color);
  }

  size_t incrementLabelCounter(Label label) { return ++labelCounter_[label]; }
  void resetLabelCounter() { labelCounter_.clear(); }

  auto generateHeatmap(float value)
  {
    float red{ value <= 0.57147f ? 446.22f * value / 255.f : 1.f };
    float green{ value <= 0.95f ? ((value - 0.6f) * 728.57f) / 255.f : 1.f };
    float blue{ 0.f };
    return Color{ red, green, blue, /*default value for alpha channel */ };
  }

private:
  Annotator() = default;
  ~Annotator() = default;
  Annotator(Annotator&&) = delete;
  Annotator(const Annotator&) = delete;
  Annotator&& operator=(Annotator&&) = delete;
  Annotator& operator=(const Annotator&) = delete;
};
