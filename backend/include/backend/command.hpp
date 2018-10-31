#pragma once

#include <memory>
#include <string>

class Command
{
public:
  virtual ~Command() = default;
  // executes the command
  void execute();
  // undoes the command
  void undo();
  // create a polymorphic copy of the command
  Command* clone() const;
  // Deletes commands. This should only be overridden in plugins. By default,
  // simply deletes command. In plugins, delete must happen in the plugin.
  virtual void deallocate();

protected:
  Command(){};
  Command(const Command&) = default;

private:
  // this function is not pure virtual because a command that needs no
  // preconditions shouldn't be forced to check for any, thus, this defaults to
  // not throwing
  virtual void checkPreconditionsImpl() const;

  virtual void executeImpl() noexcept = 0;
  virtual void undoImpl() noexcept = 0;
  virtual Command* cloneImpl() const = 0;

  Command(Command&&) = delete;
  Command& operator=(Command&&) = delete;
  Command& operator=(const Command&) = delete;
};

// facilitates the use of the command pattern
inline void
CommandDeleter(Command* p)
{
  p->deallocate();
}

using CommandPtr = std::unique_ptr<Command, decltype(&CommandDeleter)>;

template <typename T, typename... Args>
auto
MakeCommandPtr(Args&&... args)
{
  return CommandPtr{ new T{ std::forward<Args>(args)... }, &CommandDeleter };
}

inline auto
MakeCommandPtr(Command* p)
{
  return CommandPtr{ p, &CommandDeleter };
}
