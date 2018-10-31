#include "backend/commandDispatcher.hpp"
#include "backend/coreCommands.hpp"
#include "backend/observerProxies.hpp"
#include "gui/application.hpp"
#include "gui/mainWindow.hpp"
#include "utilities/auxiliary.inl"
#include "utilities/dataHandler.hpp"
#include "utilities/observer.hpp"

int
main(int argc, char* argv[])
{
  Application app{ argc, argv };

  MainWindow gui{};
  CommandDispatcher cmdDispatcher{ gui };
  RegisterCoreCommands(gui);

  gui.attachObserverToEvent(
    UserInterface::CommandIssuedEvent,
    std::make_unique<CommandIssuedObserver>(cmdDispatcher));

  //  gui.attachObserverToEvent(UserInterface::ArchivingEvent,
  //                            std::make_unique<ArchiveObserver>(cmdDispatcher));

  //  //  for (auto e : gui.listEvents()) {
  //  //    std::cout << "EVENT: " << e << std::endl;
  //  //    for (auto o : gui.listEventObservers(e))
  //  //      std::cout << "Observer " << o << std::endl;
  //  //  }

  DataHandler::Instance().attachObserverToEvent(
    DataHandler::SequenceStepEvent,
    std::make_unique<SequenceStepObserver>(gui));

  DataHandler::Instance().snapshot(0);

  //  //  for (auto e : DataHandler::Instance().listEvents()) {
  //  //    std::cout << "EVENT: " << e << std::endl;
  //  //    for (auto o : DataHandler::Instance().listEventObservers(e))
  //  //      std::cout << "Observer " << o << std::endl;
  //  //  }

  gui.show();

  return app.exec();
}
