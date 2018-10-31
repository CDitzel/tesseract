#pragma once
#include <QGLViewer/manipulatedFrame.h>
#include <models/radarModel.hpp>
#include <models/sensorModels.hpp>
#include <utilities/annotations.hpp>

namespace tsr = Tesseract;
// namespace anno = Tesseract::Annotation;

struct RadarVoxel
{
  RadarVoxel(const Shell& shell, Label label = Label::Unlabeled)
    : shell_{ shell }
    , label_{ label }
  {
  }
  const Shell& shell_; // Favor composition over inheritance
  //  CellGrid cellGrid_; // Favor composition over inheritance
  const Label label_{ Label::Unlabeled };
};

static void // RadarVoxel are drawable
  process(const RadarVoxel& voxel)
{
  glPushMatrix();
  auto const& [r_, g_, b_, a_] {
    Annotator::Instance().labelColors().at(voxel.label_)
  };

  const auto& [v0, v1, v2, v3, v4, v5, v6, v7] { voxel.shell_.vertices_ };

  // Faces
  glEnable(GL_BLEND);
  glColor4f(r_, g_, b_, .1f);
  glBegin(GL_QUADS);
  // front face
  glVertex3f(v0.x_, v0.y_, v0.z_);
  glVertex3f(v4.x_, v4.y_, v4.z_);
  glVertex3f(v7.x_, v7.y_, v7.z_);
  glVertex3f(v3.x_, v3.y_, v3.z_);
  // back face
  glVertex3f(v2.x_, v2.y_, v2.z_);
  glVertex3f(v6.x_, v6.y_, v6.z_);
  glVertex3f(v5.x_, v5.y_, v5.z_);
  glVertex3f(v1.x_, v1.y_, v1.z_);
  // top face
  glVertex3f(v4.x_, v4.y_, v4.z_);
  glVertex3f(v7.x_, v7.y_, v7.z_);
  glVertex3f(v6.x_, v6.y_, v6.z_);
  glVertex3f(v5.x_, v5.y_, v5.z_);
  // bottom face
  glVertex3f(v0.x_, v0.y_, v0.z_);
  glVertex3f(v3.x_, v3.y_, v3.z_);
  glVertex3f(v2.x_, v2.y_, v2.z_);
  glVertex3f(v1.x_, v1.y_, v1.z_);
  // left face
  glVertex3f(v2.x_, v2.y_, v2.z_);
  glVertex3f(v3.x_, v3.y_, v3.z_);
  glVertex3f(v7.x_, v7.y_, v7.z_);
  glVertex3f(v6.x_, v6.y_, v6.z_);
  // right face
  glVertex3f(v0.x_, v0.y_, v0.z_);
  glVertex3f(v1.x_, v1.y_, v1.z_);
  glVertex3f(v5.x_, v5.y_, v5.z_);
  glVertex3f(v4.x_, v4.y_, v4.z_);
  glEnd();

  // Mesh
  glLineWidth(1.5);
  glBegin(GL_LINES);
  glColor4f(0.f, 0.f, 0.f, .5f);
  glEnable(GL_LINE_SMOOTH); /* Enable Antialiased lines */

  glVertex3f(v0.x_, v0.y_, v0.z_);
  glVertex3f(v1.x_, v1.y_, v1.z_);

  glVertex3f(v1.x_, v1.y_, v1.z_);
  glVertex3f(v2.x_, v2.y_, v2.z_);

  glVertex3f(v2.x_, v2.y_, v2.z_);
  glVertex3f(v3.x_, v3.y_, v3.z_);

  glVertex3f(v3.x_, v3.y_, v3.z_);
  glVertex3f(v0.x_, v0.y_, v0.z_);

  glVertex3f(v4.x_, v4.y_, v4.z_);
  glVertex3f(v5.x_, v5.y_, v5.z_);

  glVertex3f(v5.x_, v5.y_, v5.z_);
  glVertex3f(v6.x_, v6.y_, v6.z_);

  glVertex3f(v6.x_, v6.y_, v6.z_);
  glVertex3f(v7.x_, v7.y_, v7.z_);

  glVertex3f(v7.x_, v7.y_, v7.z_);
  glVertex3f(v4.x_, v4.y_, v4.z_);

  glVertex3f(v0.x_, v0.y_, v0.z_);
  glVertex3f(v4.x_, v4.y_, v4.z_);

  glVertex3f(v1.x_, v1.y_, v1.z_);
  glVertex3f(v5.x_, v5.y_, v5.z_);

  glVertex3f(v2.x_, v2.y_, v2.z_);
  glVertex3f(v6.x_, v6.y_, v6.z_);

  glVertex3f(v3.x_, v3.y_, v3.z_);
  glVertex3f(v7.x_, v7.y_, v7.z_);

  glEnd();

  glDisable(GL_LINE_SMOOTH);
}
