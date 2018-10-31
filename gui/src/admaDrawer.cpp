#include <gui/admaDrawer.hpp>

void
AdmaDrawer::update(const AdmaModel& admaModel)
{
  //  admaModel_ = &admaModel;
}

void
AdmaDrawer::draw()
{
  if (!admaModel_)
    return;
  glLineWidth(2.5f);
  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINE_STRIP);
  glClear(GL_COLOR_BUFFER_BIT);
  //  auto carColor = admaModel_->egoVehicle_.getCarColor();
  //  glColor3f(carColor.red, carColor.green, carColor.blue);
  //  for (auto const& carPos : admaModel_->egoVehicle_.getCarShape()) {
  //    glVertex3f(carPos.x, carPos.y, .5f);
}
//  glEnd();
//  glDisable(GL_LINE_SMOOTH);
//}
