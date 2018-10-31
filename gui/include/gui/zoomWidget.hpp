#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QWheelEvent>

// Default zoom factors
constexpr double _DEF_ZOOM_FACTOR_{ 1.15 };
constexpr double _DEF_CTRL_ZOOM_FACTOR_{ 1.01 };
constexpr int _MAX_ZOOM_{ 20 };
/*!
 * By overloading this class, it is possible to draw over the image
 * (drawOnImage) or in the view port (drawInViewPort)
 */
class ZoomWidget : public QGraphicsView
{
  Q_OBJECT

public:
  // For contextual menu or need to create a custom one, set to false
  ZoomWidget(bool hasContextualMenu = true, QWidget* parent = nullptr);
  ~ZoomWidget();

  void setImage(const QImage& image);

  // Size of the image to display the raw data MUST be compliant with the size
  void setImageFromRawData(const uchar* data, int width, int height,
                           bool mirrorHorizontally = false,
                           bool mirrorVertically = false);
  // Set the zoom factor (>1) when the CTRL key is not pressed
  void setZoomFactor(const double factor) { zoomFactor = factor; }
  // Set the zoom factor (>1) when the CTRL key is pressed
  void setZoomCtrlFactor(const double factor) { zoomCtrlFactor = factor; }

public slots:
  // fits tthe image in the view while keeping aspect ratio
  void fitImage();

protected:
  /*!
   * \brief drawOnImage           Overload this function to draw over the image
   * \param painter               Painter for drawing
   * \param imageSize             Size of the image
   */
  virtual void drawOnImage(QPainter* painter, QSize imageSize);

  /*!
   * \brief drawInViewPort        Overload this function to draw in view port
   * \param painter               Painter for drawing
   * \param imageSize             Size of the view port
   */
  virtual void drawInViewPort(QPainter* painter, QSize portSize);

  /*!
   * \brief drawForeground        Drawing he foreground of the scene
   *                              When overloading this function, you must call
   *                              it parent before
   *                              exiting the function, otherwise drawOnImage
   *                              and drawInViewPort
   *                              will not work properly
   * \param painter               Painter for drawing
   * \param rect                  The area of the scene displayed in the view
   * port
   */
  virtual void drawForeground(QPainter* painter, const QRectF& rect);

  /*!
   * \brief setToolTipText        Display the tool tip over the mouse
   * \param imageCoordinates      Coordinates of the mouse in the image's frame
   * \return                      The function returns the QString that is
   *                              displayed over the image
   */
  virtual QString setToolTipText(QPoint imageCoordinates);

  /*!
   * \brief mousePressEvent       Overload this function to process mouse button
   *                              pressed
   * \param event                 mouse event
   */
  virtual void mousePressEvent(QMouseEvent* event);

  /*!
   * \brief mouseReleaseEvent     Overload this function to process mouse button
   *                              released
   * \param event                 mouse event
   */
  virtual void mouseReleaseEvent(QMouseEvent* event);

  /*!
   * \brief wheelEvent            Overload this function to process mouse wheel
   *                              event
   * \param event                 mouse wheel event
   */
  virtual void wheelEvent(QWheelEvent* event);

  /*!
   * \brief mouseMoveEvent        Overload this function to process mouse moves
   * \param event                 mouse move event
   */
  virtual void mouseMoveEvent(QMouseEvent* event);

  /*!
   * \brief resizeEvent        Overload this function to process the window
   *                           resizing event
   * \param event              resize event
   */
  virtual void resizeEvent(QResizeEvent* event);

protected slots:

  /*!
   * \brief showContextMenu       Display the contextual menu (on right click)
   * \param pos                   Position of the mouse in the widget
   */
  virtual void showContextMenu(const QPoint& pos);

private:
  // Scene where the image is drawn
  QGraphicsScene* scene;

  // Pixmap item containing the image
  QGraphicsPixmapItem* pixmapItem;

  // Size of the current image
  QSize imageSize;

  // Zoom factor
  double zoomFactor;

  // Zoom factor when the CTRL key is pressed
  double zoomCtrlFactor;

  // Accumulates the zoomfactor for the zooming
  double zoomAccumulation{ 1 };

  // Current pixmap
  QPixmap pixmap;
};
