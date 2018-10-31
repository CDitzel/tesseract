#include "gui/mainWindow.hpp"
#include <QString>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{ parent }
  , imageWidget_{ new ImageWidget{ this } }
  , ctrlWidget_{ new ControlWidget{ this } }
  , currentState_{ new CurrentState{ this } }
  , hSplit{ new QSplitter{ Qt::Orientation::Vertical } }
  , vSplit1{ new QSplitter{ hSplit } }
  , vSplit2{ new QSplitter{ hSplit } }
  //  , canvases_{new Canvas{ *currentState_, 0, vSplit1 }}
  //  , canvasSide_{ new Canvas{ *currentState_, 0, vSplit1 } }
  , canvasTop_{ new Canvas{ *currentState_, 0, vSplit1 } }
  //  , canvasFront_{ new Canvas{ *currentState_, 2, vSplit2 } }
  , canvasPersp_{ new Canvas{ *currentState_, 1, vSplit1 } }
  , historyStackWidget_{ new QListWidget{ this } }
{
  ctrlWidget_->onSetupAppearance(std::size(DataHandler::Instance()));
  createLayout();
  establishConnections();
  setFixedSize(2000, 1200);
};

void
MainWindow::showMessage(const std::string& m)
{
  statusBar()->showMessage(QString::fromStdString(m), 1000);
};

void
MainWindow::scrollSequence(const SubdirectoryHandler& snapshot)
{
  historyStackWidget_->clear();
  //  canvasSide_->updateModelView(snapshot);
  currentState_->update(snapshot);
}

void
MainWindow::updateAppearance()
{
  //  ctrlWidget_->onUpdateAppearance(ec);
  //  state_->updateState(error);
  // ... other methods
}

void
MainWindow::archive()
{
  //    DataHandler::Instance().wipeArchive();
  //    currentState_->archiveHistory();
}

void
MainWindow::createLayout()
{
  QMainWindow::setStatusBar(statusBar());
  //  setCentralWidget(canvasSide_);
  setCentralWidget(hSplit);
  //  QWidget* q = new QWidget{ this };

  //  QGridLayout* grid = new QGridLayout{ this };
  //  grid->addWidget(canvasFront_, 0, 0);
  //  grid->addWidget(canvasFront_, 1, 0);
  //  grid->addWidget(canvasFront_, 0, 2);
  //  grid->addWidget(canvasFront_, 1, 1);
  //  q->setLayout(grid);
  //  setCentralWidget(q);

  auto imgDockWidget_{ new QDockWidget{ "Camera Image" } };
  imgDockWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  imgDockWidget_->setAllowedAreas(Qt::LeftDockWidgetArea);
  imgDockWidget_->setWidget(imageWidget_);
  //  addDockWidget(Qt::LeftDockWidgetArea, imgDockWidget_);

  auto historyStackDockWidget_{ new QDockWidget{ "History Stack" } };
  historyStackDockWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  historyStackDockWidget_->setAllowedAreas(Qt::RightDockWidgetArea);
  historyStackDockWidget_->setWidget(historyStackWidget_);
  addDockWidget(Qt::RightDockWidgetArea, historyStackDockWidget_);

  auto ctrlDockWidget_{ new QDockWidget{ "Controls" } };
  ctrlDockWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  ctrlDockWidget_->setWidget(ctrlWidget_);
  addDockWidget(Qt::BottomDockWidgetArea, ctrlDockWidget_);
}

void
MainWindow::establishConnections()
{
  connect(ctrlWidget_, &ControlWidget::commandEntered, currentState_,
          &CurrentState::onCommandEntered);
  connect(currentState_, &CurrentState::command, [this](auto cmd) {
    UserInterface::raise(UserInterface::CommandIssuedEvent,
                         std::make_shared<CommandIssued>(cmd));
  });
  connect(currentState_, &CurrentState::updateAppearance, ctrlWidget_,
          &ControlWidget::onUpdateAppearance);
  connect(currentState_, &CurrentState::updateHistoryStack,
          [this](std::string const& label) {
            historyStackWidget_->addItem(QString::fromStdString(label));
          });
  connect(currentState_, &CurrentState::removeItemFromHistoryStack, [this]() {
    historyStackWidget_->takeItem(historyStackWidget_->count() - 1);
  }); // using lambdas instead of simple dispatch slots
  connect(currentState_, &CurrentState::restoreItemInHistoryStack,
          [this](const auto& label) {
            historyStackWidget_->addItem(QString::fromStdString(label));
          });
}

void
MainWindow::closeEvent(QCloseEvent* event)
{
  currentState_->archiveHistory();
  serialize(DataHandler::Instance());
  QWidget::closeEvent(event);
};
