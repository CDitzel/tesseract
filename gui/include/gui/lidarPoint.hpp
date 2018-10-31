#pragma once
#include <QGLViewer/qglviewer.h>
#include <utilities/annotations.hpp>
#include <utilities/auxiliary.inl>

namespace tsr = Tesseract;

struct LidarPoint
{
  LidarPoint(const tsr::Point3D& point3D, Label label = Label::TrafficSign)
    : point3D_{ point3D }
    , label_{ label }
  {
  }
  const tsr::Point3D point3D_;
  const Label label_;
};

static void
process(const LidarPoint& lidarPoint)
{
  //  glPushMatrix();

  //  if (lidarPoint.label_ == Label::Truck) {
  //    std::cerr << "LABEL " << lidarPoint.label_ << std::endl;
  //    std::cerr << "Red: " << r_ << " Green: " << g_ << " Blue: " << b_
  //              << std::endl;
  //  }
  //  glLineWidth(15.f);
  //  glBegin(GL_LINES);
  glPointSize(2.f);
  auto const& [r_, g_, b_, a_] {
    Annotator::Instance().labelColors().at(lidarPoint.label_)
  };
  glBegin(GL_POINTS);
  const auto& [x, y, z, label, intensity, id] { lidarPoint.point3D_ };
  glColor4f(r_, g_, b_, 1.f);
  //  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(x - 5.5, y, z);
  //  glVertex3f(x, y - 0.1, z - 0.1);
  //  glRasterPos3f(x, y, z);
  glEnd();
}
