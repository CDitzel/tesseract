#pragma once
#include <QProgressBar>
#include <QRadioButton>
#include <QWidget>
#include <gui/commandButton.hpp>
#include <gui/currentState.hpp>
#include <string>

class ControlWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ControlWidget(QWidget* parent = nullptr);

private:
  void createLayout();
  void establishConnections();

  //  const CurrentState& guiModel_;
  CommandButton* fwdBtn_;
  CommandButton* bwdBtn_;
  QRadioButton* radarFarBtn_;
  QRadioButton* radarNearBtn_;

  QProgressBar* progBar_;

signals:
  void commandEntered(std::string);
  void switchRange(tsr::ARS300);

public slots:
  void onUpdateAppearance(size_t = 0);
  void onSetupAppearance(size_t = 0);
};
