#include "utilities/auxiliary.inl"
#include "utilities/dataHandler.hpp"
#include <QApplication>
#include <QDebug>
#include <QEvent>

class Application final : public QApplication
{
  Q_OBJECT
public:
  Application(int& argc, char** argv)
    : QApplication(argc, argv)
  {
  }

  bool notify(QObject* receiver, QEvent* event) override
  {
    try {
      return QApplication::notify(receiver, event);
    } catch (Exception& e) {
      std::cerr << e.what() << std::endl;
      //      serialize(DataHandler::Instance());
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    } catch (...) {

      //      serialize(DataHandler::Instance());
      std::cerr << "Caught exception without specified handler" << '\n';
    }

    return false;
  }
};
