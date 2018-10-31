#include "backend/commandRepository.hpp"
#include "backend/command.hpp"
#include "utilities/exception.hpp"
#include <sstream>
#include <unordered_map>

class CommandRepository::CommandRepositoryImpl
{
public:
  CommandRepositoryImpl();
  void registerCommand(const std::string& name, CommandPtr c);
  CommandPtr deregisterCommand(const std::string& name);
  size_t getNumberCommands() const { return std::size(repository_); }
  CommandPtr allocateCommand(const std::string& name) const;
  bool isCommandRegistered(const std::string& s) const;
  std::set<std::string> getAllCommandNames() const;
  void clearAllCommands();

private:
  using Repository = std::unordered_map<std::string, CommandPtr>;
  Repository repository_;
};

CommandRepository::CommandRepositoryImpl::CommandRepositoryImpl() = default;

bool
CommandRepository::CommandRepositoryImpl::isCommandRegistered(
  const std::string& s) const
{
  return repository_.find(s) != cend(repository_);
}

std::set<std::string>
CommandRepository::CommandRepositoryImpl::getAllCommandNames() const
{
  std::set<std::string> commandNames;
  for (auto cmd = repository_.begin(); cmd != repository_.end(); ++cmd)
    commandNames.insert(cmd->first);
  return commandNames;
}

void
CommandRepository::CommandRepositoryImpl::clearAllCommands()
{
  repository_.clear();
}

void
CommandRepository::CommandRepositoryImpl::registerCommand(
  const std::string& commandName, CommandPtr command)
{
  if (isCommandRegistered(commandName))
    throw Exception{ "Command " + commandName + " already registered" };
  else
    repository_.emplace(commandName, std::move(command));
}

CommandPtr
CommandRepository::CommandRepositoryImpl::deregisterCommand(
  const std::string& commandName)
{
  if (isCommandRegistered(commandName)) {
    auto cmd = repository_.find(commandName);
    auto command = MakeCommandPtr(cmd->second.release());
    repository_.erase(cmd);
    return command;
  } else
    return MakeCommandPtr(nullptr);
}

CommandPtr
CommandRepository::CommandRepositoryImpl::allocateCommand(
  const std::string& commandName) const
{
  if (isCommandRegistered(commandName)) {
    const auto& command = repository_.find(commandName)->second;
    return MakeCommandPtr(command->clone());
  } else
    return MakeCommandPtr(nullptr);
}

/*!
 * \brief CommandRepository::CommandRepository
 */
CommandRepository::CommandRepository()
  : pimpl_{ std::make_unique<CommandRepositoryImpl>() }
{
}

CommandRepository::~CommandRepository()
{
}

CommandRepository&
CommandRepository::Instance()
{
  //  static int ctr = 0;
  //  std::cout << "Repository Instance CALL " << ++ctr << std::endl;
  static CommandRepository instance{};
  return instance;
}

void
CommandRepository::registerCommand(const std::string& name, CommandPtr c)
{
  pimpl_->registerCommand(name, std::move(c));
}
CommandPtr
CommandRepository::deregisterCommand(const std::string& name)
{
  return pimpl_->deregisterCommand(name);
}

size_t
CommandRepository::getNumberCommands() const
{
  return pimpl_->getNumberCommands();
}

CommandPtr
CommandRepository::allocateCommand(const std::string& name) const
{
  return pimpl_->allocateCommand(name);
}

bool
CommandRepository::isCommandRegistered(const std::string& s) const
{
  return pimpl_->isCommandRegistered(s);
}

std::set<std::string>
CommandRepository::getAllCommandNames() const
{
  return pimpl_->getAllCommandNames();
}

void
CommandRepository::clearAllCommands()
{
  pimpl_->clearAllCommands();
}
