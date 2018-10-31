#pragma once
#include "utilities/egoVehicle.hpp"
#include "utilities/subdirectoryHandler.hpp"

class AdmaModel
{
public:
  AdmaModel(const SubdirectoryHandler::EgoPositions&);

private:
  double egoPositionEast;
  double egoPositionNorth;
  double egoPositionYaw;

  //  EgoVehicle egoVehicle_;
};
