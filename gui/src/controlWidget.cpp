#include "gui/controlWidget.hpp"
#include <QHBoxLayout>

ControlWidget::ControlWidget(QWidget* parent)
  : QWidget{ parent }
  , bwdBtn_{ new CommandButton{ "<", "bwd", this } }
  , fwdBtn_{ new CommandButton{ ">", "fwd", this } }
  , progBar_{ new QProgressBar{ this } }
  , radarFarBtn_{ new QRadioButton{ "Far-Range", this } }
  , radarNearBtn_{ new QRadioButton{ "Near_Range", this } }
{
  bwdBtn_->registerShortcut(Qt::Key_Left);
  fwdBtn_->registerShortcut(Qt::Key_Right);
  radarFarBtn_->setChecked(true);
  createLayout();
  onUpdateAppearance();
  establishConnections();
}

void
ControlWidget::createLayout()
{
  auto layout{ new QHBoxLayout{ this } };
  layout->addWidget(bwdBtn_);
  layout->addWidget(fwdBtn_);
  layout->addWidget(radarFarBtn_);
  layout->addWidget(radarNearBtn_);
  layout->addWidget(progBar_);
  setLayout(layout);
}

void
ControlWidget::onUpdateAppearance(size_t id)
{
  bwdBtn_->setEnabled(true);
  fwdBtn_->setEnabled(true);
  if (id == 0) {
    bwdBtn_->setDisabled(true);
  } else if (id == std::size(DataHandler::Instance()) - 1) {
    fwdBtn_->setDisabled(true);
  }
  progBar_->setValue(id);
}

void
ControlWidget::onSetupAppearance(size_t snapshotCount)
{
  progBar_->setRange(0, snapshotCount - 1);
}

void
ControlWidget::establishConnections()
{ // just forwarding
  connect(bwdBtn_, &CommandButton::clicked, this,
          &ControlWidget::commandEntered);
  connect(fwdBtn_, &CommandButton::clicked, this,
          &ControlWidget::commandEntered);
  connect(radarFarBtn_, &QRadioButton::clicked,
          [this]() { emit switchRange(tsr::ARS300::FRS); });
  connect(radarNearBtn_, &QRadioButton::clicked,
          [this]() { emit switchRange(tsr::ARS300::NRS); });
}
