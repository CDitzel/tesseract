#pragma once

#include <QPushButton>
#include <QWidget>
#include <string>

/*!
 * \brief The CommandButton class implements the facade pattern for equipping a
 * QPushButton with additional functionality while at the same time constraining
 * its usual public interface through composition rather than  obeying LSP
 */
class CommandButton : public QWidget
{
  Q_OBJECT
public:
  CommandButton(const std::string& buttonName, const std::string& command,
                QWidget* parent = nullptr);

  void registerShortcut(const QKeySequence& sequence);

signals:
  void clicked(std::string command);

private:
  void createLayout();
  void establishConnections();

  const std::string command_;
  QPushButton* button_;
};
