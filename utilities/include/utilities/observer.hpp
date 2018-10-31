#pragma once

#include <memory>
#include <string>

class Event;

class Observer
{
public:
  explicit Observer(std::string name);
  virtual ~Observer() = default;
  Observer(Observer&&) noexcept = default;
  Observer(const Observer&) = default;
  Observer& operator=(Observer&&) noexcept = default;
  Observer& operator=(const Observer&) = default;

  void notify(std::shared_ptr<Event>&) const;

  const std::string name() const { return observerName_; }

private:
  virtual void notifyImpl(std::shared_ptr<Event>) const = 0;

  const std::string observerName_;
};
