#pragma once
//#include "gui/currentState.hpp"
#include "gui/zoomWidget.hpp"
#include <QDialog>
#include <QFileDialog>
#include <QGuiApplication>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QScrollArea>
#include <QStandardPaths>
#include <QWidget>
#include <iostream>

class ImageWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ImageWidget(QWidget* parent = nullptr);
  //  void update(const std::any&) override;
  //  void setModel(std::shared_ptr<ImageModel> model);

protected:
  void contextMenuEvent(QContextMenuEvent* event) override;

private:
  void createLayout();
  void createActionsAndConnections();
  void setImage(const QImage& newImage);
  bool loadFile(const QImage& wideAngleImg, const QImage& leftStereoImg);

  //  const CurrentState& guiModel_;
  //  ModelPtr model_;
  //  std::shared_ptr<ImageModel> model_;
  QAction* openAct;
  uchar* data;
  // Widget containing the image
  ZoomWidget* wideAngleZoomWidget;
  ZoomWidget* leftStereoZoomWidget;

public slots:
  void onUpdateView();
private slots:
};
