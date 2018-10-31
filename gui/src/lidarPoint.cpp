#include "gui/lidarPoint.hpp"

// void
// LidarDrawer::update(const LidarModel& lidarModel)
//{
//  lidarModel_ = &lidarModel;
//}

//// glRasterPos3fv
// void
// LidarDrawer::draw()
//{
//  if (!lidarModel_)
//    return;
//  glBegin(GL_POINTS);
//  for (const auto & [ cloudName, cloud ] : lidarModel_->lidarPointClouds_) {
//    glPointSize(15.0);
//    glColor4f(0.8, 1.0, 1.0, 1);
//    for (const auto & [ x, y, z, label, intensity ] : cloud) {
//      glVertex3f(x, y, z);
//    }
//  }
//  glEnd();
//}
