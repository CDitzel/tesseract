#include "backend/observerProxies.hpp"
#include "utilities/dataHandler.hpp"
#include "utilities/exception.hpp"
#include "utilities/userInterface.hpp"

/*!
 * \brief CommandIssuedObserver::CommandIssuedObserver
 * \param cd
 */
CommandIssuedObserver::CommandIssuedObserver(CommandDispatcher& cd)
  : Observer{ "CommandIssuedObserver" }
  , cmdDispatcher_{ cd }
{
}

void
CommandIssuedObserver::notifyImpl(std::shared_ptr<Event> event) const
{
  auto commandEvent{ std::dynamic_pointer_cast<CommandIssued>(event) };
  if (commandEvent == nullptr)
    throw Exception{ "Could not downcast event data to a command" };
  cmdDispatcher_.executeCommand(commandEvent->command());
}

/*!
 * \brief SequenceStepObserver::SequenceStepObserver
 * \param ui
 */
SequenceStepObserver::SequenceStepObserver(UserInterface& ui)
  : Observer{ "SequenceStepObserver" }
  , ui_{ ui }
{
}

void
SequenceStepObserver::notifyImpl(std::shared_ptr<Event> event) const
{
  auto snapshotEvent{ std::dynamic_pointer_cast<SequenceStep>(event) };
  if (snapshotEvent == nullptr)
    throw Exception{ "Could not convert DataHandlerData to a sequence" };
  ui_.scrollSequence(snapshotEvent->snapshot());
}

// ErrorObserver::ErrorObserver(UserInterface& ui)
//  : Observer{ "ErrorObserver" }
//  , ui_{ ui }
//{
//}

// void
// ErrorObserver::notifyImpl(std::shared_ptr<Event> event)
//{
//  auto errorEvent = std::dynamic_pointer_cast<Error>(event);
//  if (errorEvent == nullptr)
//    throw Exception{ "Could not convert DataHandlerEvent to error data" };
//  ui_.updateAppearance(errorEvent->error());
//}

// ArchiveObserver::ArchiveObserver(CommandDispatcher& cd)
//  : Observer{ "ArchiveObserver" }
//  , cmdDispatcher_{ cd }
//{
//}

// void
// ArchiveObserver::notifyImpl(std::shared_ptr<Event> event) const
//{
//  auto archivingdEvent = std::dynamic_pointer_cast<Archiving>(event);
//  if (archivingdEvent == nullptr)
//    throw Exception{ "Could not downcast event data to a jsonObject" };
//  cmdDispatcher_.executeCommand(commandEvent->command());
//}
