#include "utilities/egoVehicle.hpp"
#include "utilities/auxiliary.hpp"
#include "utilities/dataHandler.hpp"

EgoVehicle::EgoVehicle(double x, double y, double psi, float r, float g,
                       float b)
  : position_{ x, y }
  , psi_{ psi }
  , psiSin_{ sin(psi) }
  , psiCos_{ cos(psi) }
  , carColor_{ r, g, b }
{
  calculateContour();
}

void
EgoVehicle::calculateContour()
{
  for (auto const & [ x, y ] : contourLocal_) {
    auto globalCoords{ transformCoordSystem(x, y, psiSin_, psiCos_, position_.x,
                                            position_.y,
                                            InvertTransformation::No) };
    contourGlobal_.emplace_back(
      point_t{ globalCoords.first, globalCoords.second });
  }
}
