#include "backend/command.hpp"
#include <iostream>
#include <memory>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

class CommandRepository
{
  class CommandRepositoryImpl;

public:
  static CommandRepository& Instance();

  // register a new command for the factory: throws if a command with the
  // same name already exists...deregister first to replace a command
  void registerCommand(const std::string& name, CommandPtr c);
  // deregister a command: returns the pointer to a command and subsequently
  // removes it from the internal database of commands...returns a nullptr
  // if the command does not exist
  CommandPtr deregisterCommand(const std::string& name);
  // returns the number of commands currently registered
  size_t getNumberCommands() const;

  // returns a pointer to a command without deregistering the
  // command...returns
  // a nullptr if the command does not exist
  CommandPtr allocateCommand(const std::string& name) const;

  // returns true if the command is present, false otherwise
  bool isCommandRegistered(const std::string& s) const;

  // returns a set of all the commands
  std::set<std::string> getAllCommandNames() const;

  // clears all commands; mainly needed for testing
  void clearAllCommands();

private:
  CommandRepository();
  ~CommandRepository();

  CommandRepository(CommandRepository&) = delete;
  CommandRepository(CommandRepository&&) = delete;
  CommandRepository& operator=(CommandRepository&) = delete;
  CommandRepository& operator=(CommandRepository&&) = delete;

  std::unique_ptr<CommandRepositoryImpl> pimpl_;
};
