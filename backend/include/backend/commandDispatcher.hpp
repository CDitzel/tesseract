#pragma once

#include "backend/command.hpp"
#include <memory>
#include <set>
#include <string>

class UserInterface;

class CommandDispatcher
{
  class CommandDispatcherImpl;

public:
  explicit CommandDispatcher(UserInterface& ui);
  ~CommandDispatcher();

  void executeCommand(const std::string& command);

private:
  CommandDispatcher(CommandDispatcher&&) = delete;
  CommandDispatcher(const CommandDispatcher&) = delete;
  CommandDispatcher& operator=(CommandDispatcher&&) = delete;
  CommandDispatcher& operator=(const CommandDispatcher&) = delete;
  CommandDispatcher(const CommandDispatcher&&) = delete;
  CommandDispatcher& operator=(const CommandDispatcher&&) = delete;

  std::unique_ptr<CommandDispatcherImpl> pimpl_;
};
