#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// struct point_t;

struct EgoVehicle
{
  EgoVehicle(double x, double y, double psi, float r = 1.f, float g = 0.f,
             float b = 0.f);

  // nested aggregate class, i.e. no custom ctor, no in-class initialization, It
  // has no base classes or virtual functions, all data members are public
private:
  struct point_t
  {
    double x, y;
  } position_;

  double psi_; // orientation in degree
  double psiSin_;
  double psiCos_;

  struct color_t
  {
    float red;
    float green;
    float blue;
  } carColor_;

  using outline_t = std::vector<point_t>;
  // w.r.t. ego coord system
  outline_t contourLocal_{ { -1.5f, 1.0f }, { 3.5f, 1.0f },   { 3.5f, -1.0f },
                           { 2.0f, -1.0f }, { 2.0f, 1.0f },   { 3.5f, 0.0f },
                           { 2.0f, -1.0f }, { -1.5f, -1.0f }, { -1.5f, 1.0f } };

  // aggregation, i.e. memory resides outside the class
  outline_t contourGlobal_;
  //  std::vector<point_t> contourGlobal_;

  void calculateContour();

public:
  outline_t getCarShape() const { return contourGlobal_; }
  color_t getCarColor() const { return carColor_; }
  auto getPosition() const { return std::make_tuple(psi_, psiSin_, psiCos_); }
};
