#pragma once

#include "command.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <stack>
#include <string>

using json = nlohmann::json;

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
class EnterNumber : public Command
{
public:
  explicit EnterNumber(double d);
  explicit EnterNumber(const EnterNumber&);

private:
  // adds the number to the stack
  void executeImpl() noexcept override;
  // removes the number from the stack
  void undoImpl() noexcept override;
  EnterNumber* cloneImpl() const override;

  double number_;
};

// clear the stack
// precondition: must have at least one element on the stack
class ClearStack : public Command
{
public:
  ClearStack() {}
  explicit ClearStack(const ClearStack&);

private:
  // clears the stack
  void executeImpl() noexcept override;
  // restores the stack
  void undoImpl() noexcept override;
  ClearStack* cloneImpl() const override;

  //  std::stack<double> stack_;
};

/*!
 * \brief The FetchSnapshot class
 */
class FetchSnapshot : public Command
{
public:
  explicit FetchSnapshot(size_t);
  ~FetchSnapshot() { /*std::cerr << "FETCH COMMAND DESTROYED " << std::endl;*/}

private:
  void checkPreconditionsImpl() const override;
  void executeImpl() noexcept override;
  void undoImpl() noexcept override;
  FetchSnapshot* cloneImpl() const override;

  size_t cursor_;
};

/*!
 * \brief The ArchiveJSONObject class
 */
class ArchiveJSONObject : public Command
{
public:
  explicit ArchiveJSONObject(const json& labelJSONObject);

private:
  void checkPreconditionsImpl() const override;
  void executeImpl() noexcept override;
  void undoImpl() noexcept override;
  ArchiveJSONObject* cloneImpl() const override;

  const json& labelJSONObject_;
};

/*!
 * \brief The SerializeSnapshot class
 */
class SerializeSnapshot : public Command
{
public:
  explicit SerializeSnapshot() = default;

private:
  void checkPreconditionsImpl() const override;
  void executeImpl() noexcept override;
  void undoImpl() noexcept override;
  SerializeSnapshot* cloneImpl() const override;
};

class UserInterface;
void RegisterCoreCommands(UserInterface& ui);
