#include "utilities/publisher.hpp"
#include "utilities/exception.hpp"
#include "utilities/observer.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_map>

/*!
 * \brief The Publisher::PublisherImpl class
 */
class Publisher::PublisherImpl
{
  using ObserverList =
    std::unordered_map<std::string, std::unique_ptr<Observer>>;
  using Events = std::unordered_map<std::string, ObserverList>;

public:
  PublisherImpl() = default;
  ~PublisherImpl() = default; // publisher owns its observers here
  bool attachObserverToEvent(const std::string& eventName,
                             std::unique_ptr<Observer> observer);
  std::unique_ptr<Observer> detachObserverFromEvent(
    const std::string& eventName, const std::string& observerName);
  void notify(const std::string& eventName, std::shared_ptr<Event> d) const;
  void registerEvent(const std::string& eventName);
  void registerEvents(const std::vector<std::string>& eventNames);
  std::set<std::string> listEvents() const;
  std::set<std::string> listEventObservers(const std::string& eventName) const;

private:
  std::optional<Events::const_iterator> findRegisteredEvent(
    const std::string& eventName) const;
  std::optional<Events::iterator> findRegisteredEvent(
    const std::string& eventName);

  Events events_;
};

std::optional<Publisher::PublisherImpl::Events::const_iterator>
Publisher::PublisherImpl::findRegisteredEvent(
  const std::string& eventName) const
{
  if (auto event{ events_.find(eventName) }; event != cend(events_))
    return event;
  return std::nullopt;
}

std::optional<Publisher::PublisherImpl::Events::iterator>
Publisher::PublisherImpl::findRegisteredEvent(const std::string& eventName)
{
  if (auto event{ events_.find(eventName) }; event != cend(events_))
    return event;
  return std::nullopt;
}

bool
Publisher::PublisherImpl::attachObserverToEvent(
  const std::string& eventName, std::unique_ptr<Observer> observer)
{
  if (auto maybeEvent{ findRegisteredEvent(eventName) }) {
    auto& observerList{ maybeEvent.value()->second };
    if (auto obs{ observerList.find(observer->name()) };
        obs != cend(observerList))
      return false; // move since copy-ctor is deleted
    return observerList.emplace(observer->name(), std::move(observer)).second;
  }
  return false;
}

std::unique_ptr<Observer>
Publisher::PublisherImpl::detachObserverFromEvent(
  const std::string& eventName, const std::string& observerName)
{
  if (auto maybeEvent{ findRegisteredEvent(eventName) }) {
    //  auto event{ findRegisteredEvent(eventName).value() };
    auto& observerList{ maybeEvent.value()->second };
    if (auto obs{ observerList.find(observerName) };
        obs != cend(observerList)) {
      observerList.erase(observerName);
      return std::move(obs->second);
    } else {
      std::cerr << "Cannot detach observer because observer not found";
    }
  }
}

void
Publisher::PublisherImpl::notify(const std::string& eventName,
                                 std::shared_ptr<Event> ev) const
{
  if (auto event{ findRegisteredEvent(eventName) })
    for (const auto & [ observerName, observer ] : event.value()->second)
      observer->notify(ev);
}

void
Publisher::PublisherImpl::registerEvent(const std::string& eventName)
{
  if (events_.try_emplace(eventName, ObserverList{}).second)
    ;
  //    std::cout << "Registered event " << std::quoted(eventName) << '\n';
  else
    ;
  //    std::cerr << "Event" << std::quoted(eventName) << " already
  //    registered\n";
}

void
Publisher::PublisherImpl::registerEvents(
  const std::vector<std::string>& eventNames)
{
  for (auto eventName : eventNames)
    registerEvent(eventName);
}

std::set<std::string>
Publisher::PublisherImpl::listEvents() const
{
  std::set<std::string> eventNames;
  //  for (const auto& event : events_)
  //    if (!eventNames.insert(event.first).second)
  //      std::cerr << __func__ << " failed to insert event name into set\n";
  //  return eventNames;

  for (const auto & [ eventName, observerList ] : events_)
    eventNames.insert(eventName);
  return eventNames;
}

std::set<std::string>
Publisher::PublisherImpl::listEventObservers(const std::string& eventName) const
{
  auto event{ findRegisteredEvent(eventName).value() };
  std::set<std::string> observerNames;
  for (const auto & [ observerName, observer ] : event->second)
    observerNames.emplace(observerName);
  return observerNames;
}

/*!
 * \brief Publisher::Publisher
 */
Publisher::Publisher()
{
  impl_ = std::make_unique<PublisherImpl>();
}
bool
Publisher::attachObserverToEvent(const std::string& eventName,
                                 std::unique_ptr<Observer> observer)
{
  return impl_->attachObserverToEvent(eventName, std::move(observer));
}

std::unique_ptr<Observer>
Publisher::detachObserverFromEvent(const std::string& eventName,
                                   const std::string& observerName)
{
  return impl_->detachObserverFromEvent(eventName, observerName);
}

void
Publisher::raise(const std::string& eventName, std::shared_ptr<Event> ev) const
{
  impl_->notify(eventName, ev);
}

void
Publisher::registerEvent(const std::string& eventName)
{
  impl_->registerEvent(eventName);
}

void
Publisher::registerEvents(const std::vector<std::string>& eventNames)
{
  impl_->registerEvents(eventNames);
}

std::set<std::string>
Publisher::listEvents() const
{
  return impl_->listEvents();
}

std::set<std::string>
Publisher::listEventObservers(const std::string& eventName) const
{
  return impl_->listEventObservers(eventName);
}

// std::unique_ptr requires a definition of the destructor instead
// of using the default because the destructor must appear in a scope
// in which the complete definition of the template argument for
// std::unique_ptr is known
Publisher::~Publisher() = default;
Publisher::Publisher(Publisher&& rhs) = default;
Publisher& Publisher::operator=(Publisher&& rhs) = default;
// Publisher::Publisher(const Publisher& rhs)
//  : impl_(std::make_unique<PublisherImpl>(*rhs.impl_))
//{
//   TODO: implement copy ctor and copy assignment
//  for (rhs.impl_->events_)
//  impl_->events_.emplace
//} // copy ctor
// Publisher&
// Publisher::operator=(const Publisher& rhs)
//{
//  *impl_ = *rhs.impl_;
//  return *this;
//}
