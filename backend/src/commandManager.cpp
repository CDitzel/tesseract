#include "backend/command.hpp"
#include "backend/commandManager.hpp"
#include <list>
#include <stack>
#include <vector>

using std::make_unique;
using std::stack;
using std::vector;
using std::list;

class CommandManager::CommandManagerImpl
{
public:
  virtual ~CommandManagerImpl() {}

  virtual size_t getUndoSize() const = 0;
  virtual size_t getRedoSize() const = 0;

  virtual void executeCommand(CommandPtr c) = 0;
  virtual void undo() = 0;
  virtual void redo() = 0;
};

class CommandManager::UndoRedoStackStrategy
  : public CommandManager::CommandManagerImpl
{
public:
  size_t getUndoSize() const override { return std::size(undoStack_); }
  size_t getRedoSize() const override { return std::size(redoStack_); }

  void executeCommand(CommandPtr cPtr) override;
  void undo() override;
  void redo() override;

private:
  void flushStack(stack<CommandPtr>& st)
  {
    while (!std::empty(st))
      st.pop();
  }
  stack<CommandPtr> undoStack_;
  stack<CommandPtr> redoStack_;
};

void
CommandManager::UndoRedoStackStrategy::executeCommand(CommandPtr cPtr)
{
  cPtr->execute();
  undoStack_.push(std::move(cPtr));
  flushStack(redoStack_);
}

void
CommandManager::UndoRedoStackStrategy::undo()
{
  if (getUndoSize() == 0)
    return;
  auto& cPtr = undoStack_.top();
  cPtr->undo();
  redoStack_.push(std::move(cPtr));
  undoStack_.pop();
}

void
CommandManager::UndoRedoStackStrategy::redo()
{
  if (getRedoSize() == 0)
    return;
  auto& cPtr = redoStack_.top();
  cPtr->execute();
  undoStack_.push(std::move(cPtr));
  redoStack_.pop();
}

CommandManager::CommandManager()
{

  pimpl_ = make_unique<UndoRedoStackStrategy>();
}

CommandManager::~CommandManager()
{
}

size_t
CommandManager::getUndoSize() const
{
  return pimpl_->getUndoSize();
}

size_t
CommandManager::getRedoSize() const
{
  return pimpl_->getRedoSize();
}

void
CommandManager::executeCommand(CommandPtr c)
{
  pimpl_->executeCommand(std::move(c));
}

void
CommandManager::undo()
{
  pimpl_->undo();
}

void
CommandManager::redo()
{
  pimpl_->redo();
}
