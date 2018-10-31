#include <QDockWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QSplitter>
#include <QStatusBar>
#include <gui/canvas.hpp>
#include <gui/controlWidget.hpp>
#include <gui/currentState.hpp>
#include <gui/imageWidget.hpp>
#include <string>
#include <utilities/userInterface.hpp>

class MainWindow : public QMainWindow, public UserInterface
{
  Q_OBJECT
public:
  MainWindow(QWidget* = nullptr);

  void showMessage(const std::string& m) override;
  void scrollSequence(const SubdirectoryHandler&) override;
  void updateAppearance() override;
  void archive() override;

  void closeEvent(QCloseEvent* event) override;

private:
  void createLayout();
  void establishConnections();

  ImageWidget* imageWidget_;
  ControlWidget* ctrlWidget_;
  CurrentState* currentState_;

  // Create Splitters
  QSplitter *hSplit, *vSplit1, *vSplit2;
  std::vector<Canvas*> canvases_;
  Canvas *canvasSide_, *canvasTop_, *canvasFront_, *canvasPersp_;
  QListWidget* historyStackWidget_;

  // public slots:
  //  void onCommandEntered(std::string cmd);
  //  void onShowMessage(std::string m);
};
