#include "models/stereoModel.hpp"

StereoModel::StereoModel(
  const AdmaModel& admaModel,
  const SubdirectoryHandler::LidarPointClouds& lidarPointClouds)
//  : lidarPointClouds_{ lidarPointClouds }
{

  unsigned pointIndex{ 0 };
  for (auto & [ lidar, cloud ] : lidarPointClouds) {
    std::transform(std::cbegin(cloud), std::cend(cloud),
                   std::back_inserter(stereoTransformed_),
                   [&pointIndex](auto const& point) mutable {
                     return tsr::Point3D{ point.x_,         point.y_,
                                          point.z_,         point.label_,
                                          point.intensity_, pointIndex++ };
                   });
  }

  //  std::make_move_iterator 1!!!!!!!!!!!!!!!!

  //    tsr::shell_all(cloud);
  //  lidarPointClouds[]
  //  }

  tsr::shell_it(std::size(stereoTransformed_));

  //  for (auto & [ key, value ] : lidarPointClouds)
  //    tsr::shell_it(key);

  //  ibeoTransformed_

  std::cout << "LidarModel constructed " << std::endl;
}
