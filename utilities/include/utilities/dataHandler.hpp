#pragma once
#include "utilities/annotations.hpp"
#include "utilities/archiver.hpp"
#include "utilities/publisher.hpp"
#include "utilities/subdirectoryHandler.hpp"

class Error : public Event
{
public:
  enum class ErrorConditions
  {
    NoError, // implicitly 0
    Empty,
    ExhaustedSequence
  };

  explicit Error(ErrorConditions e)
    : err_{ e }
  {
  }

  static const char* Message(ErrorConditions ec);
  const char* message() const;
  ErrorConditions error() const { return err_; }

private:
  ErrorConditions err_;
};

class SequenceStep : public Event
{
public:
  explicit SequenceStep(SubdirectoryHandler& snapshot) noexcept
    : snapshot_{ snapshot }
  {
  }
  const SubdirectoryHandler& snapshot() const { return snapshot_; }

private:
  SubdirectoryHandler& snapshot_;
};

/*!
 * \brief The DataHandler class is a slick implementation of Meyer's singleton
 * thereby making the data globally accesible while at the same time
 * constraining the number of instances to one at all times. Furthermore, it
 * employs the pimpl idiom to separate interface and implementation more clearly
 * while skimming on build times
 */
using Sequence = std::vector<SubdirectoryHandler>;

class DataHandler : private Publisher
{ // models implements-a relationship, used to embed
  // the implementation of one
  // class into the private implementation of another

public:
  static DataHandler& Instance();
  void parseDirectory(std::string const& path,
                      bool suppressChangeEvent = false);
  void snapshot(size_t cursor);
  void archive(const RadarArchiver&);
  void wipeArchive(tsr::ARS300);
  const Sequence& sequence() const;
  //  void deserialize();
  size_t size() const;
  void clear();

  // pulls privately inherited functions into the public interface
  using Publisher::attachObserverToEvent;
  using Publisher::detachObserverFromEvent;
  using Publisher::listEventObservers;
  using Publisher::listEvents;

  static const std::string SequenceStepEvent;
  static const std::string ErrorEvent;

private:
  DataHandler();
  ~DataHandler() = default;
  DataHandler(DataHandler&&) = delete;
  DataHandler(const DataHandler&) = delete;
  DataHandler&& operator=(DataHandler&&) = delete;
  DataHandler& operator=(const DataHandler&) = delete;

  class DataHandlerImpl;
  std::unique_ptr<DataHandlerImpl> dataHandlerImpl_;
};

void serialize(const DataHandler&);
