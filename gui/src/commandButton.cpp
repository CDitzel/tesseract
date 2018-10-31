#include "gui/commandButton.hpp"
#include <QShortcut>
#include <QVBoxLayout>

CommandButton::CommandButton(const std::string& buttonName,
                             const std::string& command, QWidget* parent)
  : QWidget{ parent }
  , command_{ command }
  , button_{ new QPushButton{ QString::fromStdString(buttonName), this } }
{
  createLayout();
  establishConnections();
}

void
CommandButton::registerShortcut(const QKeySequence& sequence)
{
  auto shortcut = new QShortcut(sequence, button_);
  connect(shortcut, &QShortcut::activated, this,
          [this]() { emit clicked(command_); });
}

void
CommandButton::createLayout()
{
  auto layout = new QVBoxLayout{ this };
  layout->addWidget(button_);
  //  layout->setContentsMargins(LookAndFeel::Instance().getContentsMargins());
  setLayout(layout);
}

void
CommandButton::establishConnections()
{
  //  btn_->setFont(LookAndFeel::Instance().getButtonFont());
  //  btn_->setFixedSize(LookAndFeel::Instance().getMinimumButtonSize());
  connect(button_, &QPushButton::clicked, this,
          [this]() { emit clicked(command_); });
}
