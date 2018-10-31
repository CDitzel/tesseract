#include "utilities/dataHandler.hpp"
#include "utilities/publisher.hpp"
#include <string>

/*!
 * \brief The CommandIssued class
 */
class CommandIssued : public Event
{
public:
  explicit CommandIssued(std::string cmd) noexcept : command_{ std::move(cmd) }
  {
  }
  const std::string& command() const { return command_; }

private:
  const std::string command_;
};

///*!
// * \brief The Archiving class
// */
// class Archiving : public Event
//{
// public:
//  explicit Archiving(json labelJSONOject) noexcept
//    : labelJSONObejct_{ std::move(labelJSONOject) }
//  {
//  }
//  const json& labelJSONObject() const { return labelJSONObejct_; }

// private:
//  const json labelJSONObejct_;
//};

class UserInterface : protected Publisher
{

public:
  UserInterface()
  {
    registerEvent(CommandIssuedEvent);
    //    registerEvent(ArchivingEvent);
  }
  virtual ~UserInterface() = default;

  // post a message to the user
  virtual void showMessage(const std::string& m) = 0;

  // notifies the interface of the current snapshot
  virtual void scrollSequence(const SubdirectoryHandler&) = 0;

  //  virtual void updateAppearance(Error::ErrorConditions) = 0;
  virtual void updateAppearance() = 0;

  virtual void archive() = 0;

  // pull protected functions into public interface
  using Publisher::attachObserverToEvent;
  using Publisher::detachObserverFromEvent;
  using Publisher::listEventObservers;
  using Publisher::listEvents;

  // defines the event this publisher can raise
  // note that the string is defined in main.cpp of the application since
  // class UserInterface has no implementation file
  static const std::string CommandIssuedEvent;
  //  static const std::string ArchivingEvent;
};
