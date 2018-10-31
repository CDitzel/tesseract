#include "backend/coreCommands.hpp"
#include "backend/commandRepository.hpp"
//#include "utilities/dataHandler.hpp"
#include "utilities/exception.hpp"
#include "utilities/userInterface.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

void
registerCommand(UserInterface& ui, const std::string& label, CommandPtr c)
{
  try {
    CommandRepository::Instance().registerCommand(label, std::move(c));
  } catch (Exception& e) {
    ui.showMessage(e.what());
  }
}

EnterNumber::EnterNumber(double d)
  : Command{}
  , number_{ d }
{
}

EnterNumber::EnterNumber(const EnterNumber& rhs)
  : Command{ rhs }
  , number_{ rhs.number_ }
{
}

void
EnterNumber::executeImpl() noexcept
{
  //  Stack::Instance().push(number_);
}

void
EnterNumber::undoImpl() noexcept
{
  //  Stack::Instance().pop();
}

EnterNumber*
EnterNumber::cloneImpl() const
{
  return new EnterNumber{ *this };
}

ClearStack::ClearStack(const ClearStack& rhs)
  : Command{ rhs }
//  , stack_{ rhs.stack_ }
{
}

void
ClearStack::executeImpl() noexcept
{
  //  const auto n = Stack::Instance().size();
  //  if (n == 0)
  //    return;
  //  for (auto i = 0u; i < n - 1; ++i) {
  //    stack_.push(Stack::Instance().pop(true));
  //  }

  //  stack_.push(Stack::Instance().pop(false));
}

void
ClearStack::undoImpl() noexcept
{
  //  const auto n = stack_.size();
  //  if (n == 0)
  //    return;
  //  for (auto i = 0u; i < n - 1; ++i) {
  //    Stack::Instance().push(stack_.top(), true);
  //    stack_.pop();
  //  }

  //  Stack::Instance().push(stack_.top(), false);
  //  stack_.pop();
}

ClearStack*
ClearStack::cloneImpl() const
{
  return new ClearStack{ *this };
}

/*!
 * \brief FetchSnapshot::FetchSnapshot
 * \param cursor
 */
FetchSnapshot::FetchSnapshot(size_t cursor)
  : cursor_{ cursor }
{
}

void
FetchSnapshot::checkPreconditionsImpl() const
{
  if (cursor_ == DataHandler::Instance().size())
    throw OverflowException{ "Exceeded number of snapshots" };
}

void
FetchSnapshot::executeImpl() noexcept
{
  DataHandler::Instance().snapshot(cursor_);
}

void
FetchSnapshot::undoImpl() noexcept
{
  //  const auto n = stack_.size();
  //  if (n == 0)
  //    return;
  //  for (auto i = 0u; i < n - 1; ++i) {
  //    Stack::Instance().push(stack_.top(), true);
  //    stack_.pop();
  //  }

  //  Stack::Instance().push(stack_.top(), false);
  //  stack_.pop();
}

FetchSnapshot*
FetchSnapshot::cloneImpl() const
{
  return new FetchSnapshot{ *this };
}

/*!
 * \brief ArchiveJSONObject::ArchiveJSONObject
 * \param cursor
 */
ArchiveJSONObject::ArchiveJSONObject(const json& labelJSONObject)
  : labelJSONObject_{ labelJSONObject }
{
}

void
ArchiveJSONObject::checkPreconditionsImpl() const
{
  //  DataHandler::Instance().
}

void
ArchiveJSONObject::executeImpl() noexcept
{
  //  DataHandler::Instance().archive(labelJSONObject_);
}

void
ArchiveJSONObject::undoImpl() noexcept
{
}

ArchiveJSONObject*
ArchiveJSONObject::cloneImpl() const
{
  return new ArchiveJSONObject{ *this };
}

/*!
 * \brief SerializeSnapshot::SerializeSnapshot
 * \param cursor
 */
void
SerializeSnapshot::checkPreconditionsImpl() const
{
  //  DataHandler::Instance().
}

void
SerializeSnapshot::executeImpl() noexcept
{
}

void
SerializeSnapshot::undoImpl() noexcept
{
}

SerializeSnapshot*
SerializeSnapshot::cloneImpl() const
{
  return new SerializeSnapshot{ *this };
}

void
RegisterCoreCommands(UserInterface& ui)
{
  registerCommand(ui, "clear", MakeCommandPtr<ClearStack>());
  //  registerCommand(ui, "step", MakeCommandPtr<FetchSnapshot>());
}
