#include "gui/canvas.hpp"
#include <QSplitter>
#include <gui/radarVoxel.hpp>

namespace tsr = Tesseract;
// namespace anno = Tesseract::Annotation;

Canvas::Canvas(CurrentState& currentState, int n, QWidget* parent)
  : QGLViewer{ parent }
  , currentState_{ currentState }
  , annotator_{ Annotator::Instance() /* Big Singleton blunder */ }
{
  setAxisIsDrawn();
  setFPSIsDisplayed();
  selectionMode_ = SelectionMode::NONE;
  setSelectBufferSize(500000);

  if (n < 1) {
    // Move camera according to viewer type (on X, Y or Z axis)
    camera()->setPosition(qglviewer::Vec(0, 0, 1));
    //      (n == 0) ? 1.0 : 0.0, (n == 1) ? 1.0 : 0.0, (n == 2) ? 1.0 : 0.0));
    camera()->lookAt(sceneCenter());
    //    camera()->setOrientation(90, 135);
    //    camera()->setViewDirection({ 1, 0, 0 });
    camera()->setType(qglviewer::Camera::PERSPECTIVE);
    camera()->showEntireScene();

    // Forbid rotation
    //    qglviewer::WorldConstraint* constraint = new
    //    qglviewer::WorldConstraint();
    //    constraint->setRotationConstraintType(
    //      qglviewer::AxisPlaneConstraint::FORBIDDEN);
    //    camera()->frame()->setConstraint(constraint);
  }

  annotator_.setShortcut(Qt::Key_0, Label::Unlabeled);
  annotator_.setShortcut(Qt::Key_1, Label::Sky);
  annotator_.setShortcut(Qt::Key_2, Label::Car);
  annotator_.setShortcut(Qt::Key_3, Label::Truck);
  annotator_.setShortcut(Qt::Key_4, Label::Bus);
  annotator_.setShortcut(Qt::Key_5, Label::Motorcycle);
  annotator_.setShortcut(Qt::Key_6, Label::Pedestrian);
  annotator_.setShortcut(Qt::Key_7, Label::Bicyclist);
  annotator_.setShortcut(Qt::Key_8, Label::TrafficSign);
  annotator_.setShortcut(Qt::Key_9, Label::TrafficLight);

  auto unitRGB{ tsr::scaleValue(0, 255, 1) }; // scale 8bit RGB to unit RGB

  annotator_.setLabelColor(Label::Unlabeled,
                           { unitRGB(255), unitRGB(255), unitRGB(255) });
  annotator_.setLabelColor(Label::Sky, { unitRGB(0), unitRGB(0), unitRGB(0) });
  annotator_.setLabelColor(Label::Car,
                           { unitRGB(0), unitRGB(0), unitRGB(255) });
  annotator_.setLabelColor(Label::Truck,
                           { unitRGB(0), unitRGB(255), unitRGB(255) });
  annotator_.setLabelColor(Label::Bus,
                           { unitRGB(153), unitRGB(153), unitRGB(255) });
  annotator_.setLabelColor(Label::Motorcycle,
                           { unitRGB(0), unitRGB(0), unitRGB(230) });
  annotator_.setLabelColor(Label::Pedestrian,
                           { unitRGB(220), unitRGB(20), unitRGB(60) });
  annotator_.setLabelColor(Label::Bicyclist,
                           { unitRGB(155), unitRGB(40), unitRGB(0) });
  annotator_.setLabelColor(Label::TrafficSign,
                           { unitRGB(220), unitRGB(220), unitRGB(0) });
  annotator_.setLabelColor(Label::TrafficLight,
                           { unitRGB(250), unitRGB(170), unitRGB(30) });
}

// void
// Canvas::redraw()
//{
//  update();
//}

void
Canvas::init()
{
  //  startScreenCoordinatesSystem();
  camera()->lookAt(sceneCenter());
  camera()->showEntireScene();
  camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
  camera()->setZClippingCoefficient(2500.0);
  camera()->setFOVToFitScene();
  //  drawVisualHints();
  setGridIsDrawn();
  setSceneRadius(1.f); // scene has a 100 OpenGL units radius
                       //  setSceneCenter(qglviewer::Vec{
  //    400, 0, 0 }); // with a center shifted by 400 units along X direction
  camera()->showEntireScene();
  //  glBlendFunc(GL_ONE, GL_ONE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glEnable(GL_TEXTURE_2D);

  //  glEnable(GL_LIGHT0);
  //  // Directional light
  //  float pos[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
  //  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  //  // Nice texture coordinate interpolation
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  //  restoreStateFromFile();
  // Absolutely needed for MouseGrabber
  //  setMouseTracking(true);
}

void
Canvas::updateModelView(const SubdirectoryHandler& snapshot)
{
  //  const SensorModels& sensormodels{
  const auto& message{ currentState_.update(snapshot) };
  QGLViewer::displayMessage(message, std::numeric_limits<int>::max());
  //  auto render{ tsr::multiCall(*radarDrawer_, lidarDrawer_,
  //  admaDrawer_) };

  //  auto render{ tsr::multiCall(lidarDrawer_) };
  //  render(sensorModels); // drawer instances are callables
}

void
Canvas::draw()
{
  currentState_.draw();
  if (selectionMode_ != SelectionMode::NONE)
    drawSelectionRectangle();
}

void
Canvas::drawWithNames()
{
  currentState_.drawSelection();
}

void
Canvas::mousePressEvent(QMouseEvent* e)
{
  rectangle_ = QRect{ e->pos(), e->pos() }; // top-left and bottom-right corner
  if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ShiftModifier))
    selectionMode_ = SelectionMode::ADD;
  else if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::META))
    selectionMode_ = SelectionMode::REMOVE;
  else {
    QGLViewer::mousePressEvent(e);
  }
}

void
Canvas::mouseMoveEvent(QMouseEvent* e)
{
  if (selectionMode_ != SelectionMode::NONE) {
    // Updates rectangle_ coordinates and redraws rectangle
    rectangle_.setBottomRight(e->pos());
    update();
  } else
    QGLViewer::mouseMoveEvent(e);
}

void
Canvas::mouseReleaseEvent(QMouseEvent* e)
{
  if (selectionMode_ != SelectionMode::NONE) {
    // Actual selection on the rectangular area.
    // Possibly swap left/right and top/bottom to make rectangle_ valid.
    rectangle_ = rectangle_.normalized();
    // Define selection window dimensions
    setSelectRegionWidth(rectangle_.width());
    setSelectRegionHeight(rectangle_.height());
    // Compute rectangle center and perform selection
    select(rectangle_.center());
    // Update display to show new selected objects
    update();
  } else
    QGLViewer::mouseReleaseEvent(e);
}

void
Canvas::keyPressEvent(QKeyEvent* e)
{
  const auto currentlyPressedKey{ Qt::Key(e->key()) };
  const auto guard{ tsr::safe_deref(annotator_.labelKeys()) };

  if (currentlyPressedKey == Qt::Key_U) {
    currentState_.undoHistory();
    update();
  }

  if (currentlyPressedKey == Qt::Key_R &&
      e->modifiers() == Qt::ControlModifier) {
    currentState_.redoHistory();
    update();
  }
  if (currentlyPressedKey == Qt::Key_X) {
    currentState_.onSwitchRange();
    update();
  }
  if (currentlyPressedKey == Qt::Key_W) {
    currentState_.onToggleSensorAnnotation();
    //    update();
  }

  if (auto maybe{ guard((annotator_.labelKeys().find(currentlyPressedKey))) };
      maybe.has_value()) {
    selectedLabel_ = maybe.value().second;
  }
  QGLViewer::keyPressEvent(e);
}

void
Canvas::endSelection(const QPoint&)
{
  glFlush();
  // Get the number of objects that were seen through the pick matrix frustum.
  // Reset GL_RENDER mode.

  if (GLint nbHits{ glRenderMode(GL_RENDER) }; nbHits > 0) {
    std::cerr << "NUMBER OF SELECTED OBJECTS " << nbHits << std::endl;
    // Interpret results : each object created 4 values in the selectBuffer().
    // (selectBuffer())[4*i+3] is the id pushed on the stack.
    for (auto i{ 0 }; i < nbHits; ++i) {
      unsigned _voxel_ID{ selectBuffer()[4 * i + 3] };
      //      std::cerr << "INDEX_CANVAS: " << _voxel_ID << std::endl;
      addIdToSelection(_voxel_ID);
    }
    if (bool success{ currentState_.makeHistory(selection_, selectedLabel_) };
        success) {
      selection_.clear();
    }
  }
  selectionMode_ = SelectionMode::NONE;
}

void
Canvas::addIdToSelection(int id)
{
  selection_.emplace(id);
}

void
Canvas::removeIdFromSelection(int id)
{
  selection_.erase(id);
}

void
Canvas::drawSelectionRectangle() const
{
  startScreenCoordinatesSystem();
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);

  auto const& [_r, _g, _b, _a] {
    Annotator::Instance().labelColors().at(selectedLabel_)
  };

  glColor4f(_r, _g, _b, 0.3f);
  glBegin(GL_QUADS);
  glVertex2i(rectangle_.left(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(), rectangle_.bottom());
  glEnd();

  glLineWidth(2.0);
  glColor4f(0.4f, 0.4f, 0.5f, 0.5f);
  glBegin(GL_LINE_LOOP);
  glVertex2i(rectangle_.left(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(), rectangle_.bottom());
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  stopScreenCoordinatesSystem();
}
// The thumbnail has to be drawn at the very end to allow a correct
// display of the visual hints (axes, grid, etc).

void
Canvas::postDraw()
{
  QGLViewer::postDraw();
  drawCornerAxis();
}

void
Canvas::drawCornerAxis()
{
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

// void
// modify(State& state)
//{
//  //  state
//}
