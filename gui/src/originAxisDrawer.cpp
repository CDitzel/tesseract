#include "gui/originAxisDrawer.hpp"

void
OriginAxisDrawer::draw()
{
  //  QGLViewer::postDraw();
  int viewport[4];
  int scissor[4];

  // The viewport and the scissor are changed to fit the lower left
  // corner. Original values are saved.
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetIntegerv(GL_SCISSOR_BOX, scissor);

  // Axis viewport size, in pixels
  const int size = 150;
  glViewport(0, 0, size, size);
  glScissor(0, 0, size, size);

  // The Z-buffer is cleared to make the axis appear over the
  // original image.
  glClear(GL_DEPTH_BUFFER_BIT);

  // Tune for best line rendering
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixd(camera()->orientation().inverse().matrix());

  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0);
  glEnd();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glEnable(GL_LIGHTING);

  // The viewport and the scissor are restored.
  glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}
