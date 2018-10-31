#include "backend/command.hpp"
#include <memory>

class CommandManager
{
  class CommandManagerImpl;
  class UndoRedoStackStrategy;

public:
  explicit CommandManager();
  ~CommandManager();

  size_t getUndoSize() const;
  size_t getRedoSize() const;

  // This function call executes the command, enters the new command onto the
  // undo stack,
  // and it clears the redo stack. This is consistent with typical undo/redo
  // functionality.
  void executeCommand(CommandPtr);

  // This function undoes the command at the top of the undo stack and moves
  // this command
  // to the redo stack. It does nothing if the undo stack is empty.
  void undo();

  // This function executes the command at the top of the redo stack and moves
  // this command
  // to the undo stack. It does nothing if the redo stack is empty.
  void redo();

private:
  CommandManager(CommandManager&) = delete;
  CommandManager(CommandManager&&) = delete;
  CommandManager& operator=(CommandManager&) = delete;
  CommandManager& operator=(CommandManager&&) = delete;

  std::unique_ptr<CommandManagerImpl> pimpl_;
};
