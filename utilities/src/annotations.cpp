#include <utilities/annotations.hpp>

std::ostream&
operator<<(std::ostream& os, Label label)
{
  switch (label) {
    case Label::Unlabeled:
      os << "Unlabeled";
      break;
    case Label::Sky:
      os << "Sky";
      break;
    case Label::Car:
      os << "Car";
      break;
    case Label::Truck:
      os << "Truck";
      break;
    case Label::Bus:
      os << "Bus";
      break;
    case Label::Motorcycle:
      os << "Motorcycle";
      break;
    case Label::Pedestrian:
      os << "Pedestrian";
      break;
    case Label::Bicyclist:
      os << "Bicyclist";
      break;
    case Label::TrafficSign:
      os << "Traffic_sign";
      break;
    case Label::TrafficLight:
      os << "Traffic_light";
      break;
    case Label::Road:
      os << "Road";
      break;
    case Label::RoadMarking:
      os << "Road_marking";
      break;
    case Label::Sidewalk:
      os << "Sidewalk";
      break;
    case Label::Building:
      os << "Building";
      break;
    case Label::Vegetation:
      os << "Vegetation";
      break;
    case Label::Pole:
      os << "Pole";
      break;
    case Label::Animals:
      os << "Amimals";
      break;
    case Label::LostCargo:
      os << "Lost_cargo";
      break;
    default:
      os.setstate(std::ios_base::failbit);
  }
  return os;
}

Label
stringToLabel(const std::string& s)
{
  if (s == "Unlabeled")
    return Label::Unlabeled;
  if (s == "Sky")
    return Label::Sky;
  if (s == "Car")
    return Label::Car;
  if (s == "Truck")
    return Label::Truck;
  if (s == "Bus")
    return Label::Bus;
  if (s == "Motorcycle")
    return Label::Motorcycle;
  if (s == "Pedestrian")
    return Label::Pedestrian;
  if (s == "Bicyclist")
    return Label::Bicyclist;
  if (s == "Traffic_sign")
    return Label::TrafficSign;
  if (s == "Traffic_light")
    return Label::TrafficLight;
  if (s == "Road")
    return Label::Road;
  if (s == "Road_marking")
    return Label::RoadMarking;
  if (s == "Sidewalk")
    return Label::Sidewalk;
  if (s == "Building")
    return Label::Building;
  if (s == "Vegetation")
    return Label::Vegetation;
  if (s == "Pole")
    return Label::Pole;
  if (s == "Animals")
    return Label::Animals;
  if (s == "Lost_cargo")
    return Label::LostCargo;
}

std::string
labelToString(Label label)
{
  std::stringstream _converter;
  _converter << label;
  return _converter.str();
}
