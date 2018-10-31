#pragma once

#include "backend/commandDispatcher.hpp"
#include "utilities/observer.hpp"
// The proxy pattern uses a class buffer to allow the system to perceive the
// target as an object independent of its underlying composition (dependency
// injection) rather than
// making the target inherit from the observer class. The proxy pattern buffers
// communication between publishers and observers.
// First, it increases clarity in the target class’s public interface by
// replacing a generically named notify() function with a descriptively named
// event
// handling function. Second, an otherwise unnecessary inheritance from the
// Observer class is removed. Eliminating this dependency reduces coupling,
// increases cohesion, and facilitates reuse of the target in a setting where it
// is
// not an observer. Third,
// using a proxy class eliminates the ambiguity that arises for a target class
// that
// needs to listen to multiple events. Without using proxy classes, an observer
// would be required to disambiguate events in its single notify() function.
// Using
// an individual proxy for each event enables each event to call a unique
// handler
// function in the target object. The main disadvantage of implementing the
// observer pattern using a proxy is the slight cost of one extra indirection
// for
// handling each event. However, in situations where using the observer pattern
// is
// appropriate, the cost of an extra indirection is negligible.
class UserInterface;

/*!
 * \brief The CommandIssuedObserver class acts as a proxy between the Gui as a
 * publisher and the CommandDispatcher as an observer
 */
class CommandIssuedObserver : public Observer
{
public:
  explicit CommandIssuedObserver(CommandDispatcher& cd);

private:
  void notifyImpl(std::shared_ptr<Event>) const override;

  CommandDispatcher& cmdDispatcher_;
};

/*!
 * \brief The DataHandlerUpdatedObserver class acts as a proxy between the
 * DataHandler as publisher and the Gui as an observer
 */
class SequenceStepObserver : public Observer
{
public:
  explicit SequenceStepObserver(UserInterface& ui);

private:
  void notifyImpl(std::shared_ptr<Event>) const override;

  UserInterface& ui_;
};

///*!
// * \brief The ArchiveObserver class
// */
// class ArchiveObserver : public Observer
//{
// public:
//  explicit ArchiveObserver(CommandDispatcher& cd);

// private:
//  void notifyImpl(std::shared_ptr<Event>) const override;

//  CommandDispatcher& cmdDispatcher_;
//};
