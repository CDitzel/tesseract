#pragma once
#include <string>

class Exception
{
public:
  explicit Exception(const std::string& msg)
    : msg_{ msg }
  {
  }
  const std::string& what() const noexcept { return msg_; }

private:
  std::string msg_;
};

class OverflowException : public Exception
{
public:
  explicit OverflowException(const std::string& msg)
    : Exception{ msg }
  {
  }
};

class UnderflowException : public Exception
{
public:
  explicit UnderflowException(const std::string& msg)
    : Exception{ msg }
  {
  }
};
