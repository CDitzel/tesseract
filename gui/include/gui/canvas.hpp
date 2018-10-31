#pragma once
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include <gui/admaDrawer.hpp>
#include <gui/currentState.hpp>
#include <gui/lidarPoint.hpp>
#include <gui/originAxisDrawer.hpp>
#include <gui/radarVoxel.hpp>
#include <unordered_set>
#include <utilities/annotations.hpp>

class Canvas : public QGLViewer
{
  Q_OBJECT

private:
  CurrentState& currentState_;
  //  LidarDrawer lidarDrawer_;
  Annotator& annotator_;

  enum class SelectionMode
  {
    NONE,
    ADD,
    REMOVE
  } selectionMode_;

  // Current rectangular selection
  QRect rectangle_;
  std::set<uint16_t> selection_;
  Label selectedLabel_;

  //  tsr::history_t history_{ 1 };
  //  RadarModel const* radarModel_{ nullptr };

  void drawWithNames() override;
  void endSelection(const QPoint&) override;

  void drawCornerAxis();

  void drawSelectionRectangle() const;
  void addIdToSelection(int id);
  void removeIdFromSelection(int id);

  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void keyPressEvent(QKeyEvent* e) override;

public:
  explicit Canvas(CurrentState&, int, QWidget* = nullptr);

signals:
  void updateHistoryStack(std::string);

public slots:
  //  void redraw();
  virtual void updateModelView(const SubdirectoryHandler&);
private slots:
  void draw() override;
  void init() override;
  void postDraw() override;
};
