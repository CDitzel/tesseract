#include "utilities//observer.hpp"
#include "utilities//exception.hpp"
#include "utilities/publisher.hpp"

#include <iostream>

Observer::Observer(std::string name)
  : observerName_{ std::move(name) }
{
}

/*!
 * \brief Observer::notify employs NVI and dispatches the EventData to the
 * subclass
 * \param ev the eventdata to be processed
 */
void
Observer::notify(std::shared_ptr<Event>& ev) const
{
  notifyImpl(ev);
}
