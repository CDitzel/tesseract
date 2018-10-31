#include "backend/commandDispatcher.hpp"
#include "backend/commandManager.hpp"
#include "backend/commandRepository.hpp"
#include "backend/coreCommands.hpp"
#include "utilities/exception.hpp"
#include "utilities/userInterface.hpp"
#include <regex>

class CommandDispatcher::CommandDispatcherImpl
{
public:
  explicit CommandDispatcherImpl(UserInterface& ui);

  void executeCommand(const std::string& command);

private:
  inline bool isNum(const std::string&, size_t&);
  void handleCommand(CommandPtr command);

  CommandManager manager_; // composition with default ctor
  UserInterface& ui_;
};

CommandDispatcher::CommandDispatcherImpl::CommandDispatcherImpl(
  UserInterface& ui)
  : ui_{ ui }
{
}

/*!
 * \brief CommandDispatcher::CommandDispatcherImpl::executeCommand
 * handles special commands, i.e. those that are not entered in the command repo
 * \param command
 */
void
CommandDispatcher::CommandDispatcherImpl::executeCommand(
  const std::string& commandName)
{
  size_t cursor{ 0 };
  if (isNum(commandName, cursor)) {
    // whether or not command is put onto the undo/redo stackk
    //    manager_.executeCommand(MakeCommandPtr<FetchSnapshot>(cursor));
    auto cmdPtr = MakeCommandPtr<FetchSnapshot>(cursor);
    //    try {
    //    ui_.archive();
    cmdPtr->execute();
    //    } catch (const OverflowException& e) {
    //      std::cout << e.what() << '\n';
    //      ui_.showMessage(e.what());
    //    ui_.updateAppearance();
    //    }
  } else if (commandName == "undo") {
    manager_.undo();
    if (manager_.getUndoSize())
      ui_.showMessage("No more undoes");
  } else if (commandName == "redo") {
    manager_.redo();
    if (manager_.getRedoSize())
      ui_.showMessage("No more redos");
    //  } else if (commandName == "serialize") {
    //    auto archivePtr = MakeCommandPtr<ArchiveJSONObject>(ui_.archive());
    //    archivePtr->execute();
  } else {
    auto command = CommandRepository::Instance().allocateCommand(commandName);
    if (command == nullptr) {
      ui_.showMessage("Command " + commandName + " is not a known command");
    } else
      handleCommand(std::move(command));
  }
}

/*!
 * \brief CommandDispatcher::CommandDispatcherImpl::handleCommand
 * separates the logic of command interpretation from command execution
 * \param c
 */
void
CommandDispatcher::CommandDispatcherImpl::handleCommand(CommandPtr c)
{
  try {
    manager_.executeCommand(std::move(c));
  } catch (Exception& e) {
    std::cout << e.what() << std::endl;
    ui_.showMessage(e.what());
  }
}

// uses a C++11 regular expression to check if this is a valid double number
// if so, converts it into one and returns it
bool
CommandDispatcher::CommandDispatcherImpl::isNum(const std::string& s, size_t& c)
{
  std::regex pattern{ "^0$|^[1-9][0-9]*$" };
  bool isNumber{ std::regex_match(s, pattern) };
  c = isNumber ? std::stoi(s) : std::numeric_limits<size_t>::max();
  return isNumber;
}

void
CommandDispatcher::executeCommand(const std::string& command)
{
  pimpl_->executeCommand(command);
}

CommandDispatcher::CommandDispatcher(UserInterface& ui)
{
  pimpl_ = std::make_unique<CommandDispatcherImpl>(ui);
}

CommandDispatcher::~CommandDispatcher()
{
}
