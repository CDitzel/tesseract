#pragma once
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

class Observer;

class Event
{
public:
  virtual ~Event() = default;
};

class Publisher
{

public:
  Publisher();
  bool attachObserverToEvent(const std::string& eventName,
                             std::unique_ptr<Observer> observer);
  std::unique_ptr<Observer> detachObserverFromEvent(
    const std::string& eventName, const std::string& observerName);
  std::set<std::string> listEvents() const;
  std::set<std::string> listEventObservers(const std::string& eventName) const;
  Publisher(const Publisher& rhs);
  Publisher& operator=(const Publisher& rhs);
  Publisher(Publisher&& rhs);
  Publisher& operator=(Publisher&& rhs);

protected:
  ~Publisher(); // nonvirtual, protected destructor, i.e. unusable for
                // public inheritance
  void raise(const std::string& eventName, std::shared_ptr<Event>) const;

  void registerEvent(const std::string& eventName);
  void registerEvents(const std::vector<std::string>& eventNames);

private:
  class PublisherImpl;
  std::unique_ptr<PublisherImpl> impl_;
};
