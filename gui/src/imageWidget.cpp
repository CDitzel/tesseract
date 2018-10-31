#include "gui/imageWidget.hpp"
#include <QtWidgets>
//#define LOAD_RAW_RGB_DATA
#define WIDTH 600
#define HEIGHT 400

ImageWidget::ImageWidget(QWidget* parent /*,BaseModel* cameraImageModel*/)
  : QWidget{ parent }
  , leftStereoZoomWidget{ new ZoomWidget{ true, this } }
  , wideAngleZoomWidget{ new ZoomWidget{ true, this } }
{
  createLayout();
  //  establishConnections();
  //  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
  setFixedSize(600, 500);

#ifdef LOAD_RAW_RGB_DATA
  // Prepare raw image data, format is RGBRGBRGB...
  data = new uchar[WIDTH * HEIGHT * 3];
  int index = 0;
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++) {
      data[index++] = 0xFF * (x < (2. * WIDTH / 3.)); // || 0x00;
      data[index++] = 0xFF * (x > (WIDTH / 3.) && (x < (2 * WIDTH / 3.)));
      data[index++] = 0xFF * (x > (WIDTH / 3.));
    }
  // Draw the raw image in the image widget
  zoomWidget->setImageFromRawData(data, WIDTH, HEIGHT);
#else
// Draw the Qt image in the image widget
//  zoomWidget->setImage(QImage("qt.jpg"));
#endif
  // Set background color (gray)
  leftStereoZoomWidget->setBackgroundBrush(QBrush(QColor(0x7F, 0x7F, 0x7F)));
  wideAngleZoomWidget->setBackgroundBrush(QBrush(QColor(0x7F, 0x7F, 0x7F)));
  // Add the widget in the window
}

// void
// ImageWidget::update(const std::any&)
//{
//  //  loadFile(model_->leftStereoImg_, model_->wideAngleImg_);
//}

// void
// ImageWidget::setModel(std::shared_ptr<ImageModel> model)
//{
////  model_ = model;
//}

bool
ImageWidget::loadFile(const QImage& leftStereoImg, const QImage& wideAngleImg)
{
  //  QImageReader reader(fileName);
  //  reader.setAutoTransform(true);
  //  const QImage newImage = reader.read();
  if (wideAngleImg.isNull() || leftStereoImg.isNull()) {
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                             "Cannot load image");
    return false;
  }
  leftStereoZoomWidget->setImage(leftStereoImg);
  leftStereoZoomWidget->setBackgroundBrush(QBrush(QColor(0x7F, 0x7F, 0x7F)));
  leftStereoZoomWidget->fitImage();
  wideAngleZoomWidget->setImage(wideAngleImg);
  wideAngleZoomWidget->setBackgroundBrush(QBrush(QColor(0x7F, 0x7F, 0x7F)));
  wideAngleZoomWidget->fitImage();
  return true;
}

void
ImageWidget::onUpdateView()
{
  //  const auto& state = guiModel_.getState();
  //  loadFile(state.leftStereoImg, state.wideAngleImg);
}

void
ImageWidget::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu(this);
  menu.addAction(openAct);
  menu.exec(event->globalPos());
}

void
ImageWidget::createLayout()
{
  auto layout = new QGridLayout{ this };
  layout->addWidget(leftStereoZoomWidget, 0, 0);
  layout->addWidget(wideAngleZoomWidget, 1, 0);
  setLayout(layout);
}
