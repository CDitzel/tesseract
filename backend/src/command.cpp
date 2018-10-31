#include "backend/command.hpp"
#include "utilities/exception.hpp"
#include <iostream>

void
Command::execute()
{
  checkPreconditionsImpl();
  executeImpl();
}

void
Command::undo()
{
  undoImpl();
}

Command*
Command::clone() const
{
  return cloneImpl();
}

void
Command::deallocate()
{
  delete this;
}

void
Command::checkPreconditionsImpl() const
{
}
