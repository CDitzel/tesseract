#include "utilities/dataHandler.hpp"

const std::string DataHandler::SequenceStepEvent{ "SequenceStepEvent" };
const std::string DataHandler::ErrorEvent = { "ErrorEvent" };
/*!
 * \brief DataHandlerEventData::Message
 * \param ec
 * \return
 */
const char*
Error::Message(Error::ErrorConditions ec)
{
  switch (ec) {
    case ErrorConditions::Empty:
      return "Attempting to pop empty stack";
    case ErrorConditions::ExhaustedSequence:
      return "Reached the beginning or end of sequence";
    default:
      return "Unknown error";
  };
}

const char*
Error::message() const
{
  return Message(err_);
}

/*!
 * \brief The DataHandler::DataHandlerImpl class
 */
class DataHandler::DataHandlerImpl final
{

public:
  explicit DataHandlerImpl(DataHandler const&);
  void parseDirectory(std::string const& path);
  void snapshot(size_t cursor);
  void archive(const RadarArchiver&);
  void wipeArchive(tsr::ARS300);
  const Sequence& sequence() const;
  size_t size() const { return std::size(sequence_); }
  void clear();

private:
  size_t snapshotIndex_{ 0 };
  const DataHandler& parent_;
  Sequence sequence_;
  fs::path sequenceDirPath_;
  std::set<fs::path> subDirectoryPaths_;
};

DataHandler::DataHandlerImpl::DataHandlerImpl(const DataHandler& p)
  : parent_{ p }
{
}

void
DataHandler::DataHandlerImpl::parseDirectory(const std::string& path)
{
  sequenceDirPath_ = path;
  if (fs::is_empty(sequenceDirPath_))
    throw Exception{ "Empty Dir" };
  if (!fs::exists(sequenceDirPath_))
    throw Exception{ "nonexisting Dir" };
  if (!fs::is_directory(sequenceDirPath_))
    throw Exception{ "Not a Directory" };
  // red flags flying!!!
  //        on Linux listing isn't sorted. That's because the ordering of
  //        directory iteration is unspecified. Ordering depends on the
  //        underlying operating system API and file system specifics.
  for (const auto& subdir : fs::directory_iterator{ sequenceDirPath_ }) {
    subDirectoryPaths_.emplace(subdir); // std::set to the rescue;
  }
  for (const auto& subPath : subDirectoryPaths_)
    sequence_.emplace_back(subPath, snapshotIndex_++);
  snapshotIndex_ = 0;
  //  std::cout << sequenceDirPath_ << " is a directory containing: " <<
  //  std::endl;
  //  for (const auto& x : sequence_) {
  //    std::cout << " --> " << x.snapshotDirPath_ << std::endl;
  //  }
}

void
DataHandler::DataHandlerImpl::snapshot(size_t cursor)
{
  //  sequence_.at(snapshotIndex_).serializeJSON();
  snapshotIndex_ = cursor;
  //  sequence_.at(snapshotIndex_).deserializeJSON();
  parent_.raise(DataHandler::SequenceStepEvent,
                std::make_shared<SequenceStep>(sequence_.at(snapshotIndex_)));
}

void
DataHandler::DataHandlerImpl::archive(const RadarArchiver& archiver)
{
  sequence_.at(snapshotIndex_).archive(archiver);
}

void
DataHandler::DataHandlerImpl::wipeArchive(tsr::ARS300 _mode)
{
  sequence_.at(snapshotIndex_).wipeArchive(_mode);
}

void
DataHandler::DataHandlerImpl::clear()
{
  sequence_.clear();
}

const Sequence&
DataHandler::DataHandlerImpl::sequence() const
{
  return sequence_;
}

/*!
 * \brief DataHandler::Instance         thread-safe because of the C++11-
 *                                      standard guarantees that a static
 *                                      variable with block scope will
 *                                      be initialised in a thread-safe way
 * \return
 */
DataHandler&
DataHandler::Instance()
{
  static DataHandler instance{};
  return instance;
}

void
DataHandler::parseDirectory(const std::string& path, bool suppressChangeEvent)
{
  dataHandlerImpl_->parseDirectory(path);
}

void
DataHandler::snapshot(size_t cursor)
{
  dataHandlerImpl_->snapshot(cursor);
}

void
DataHandler::archive(const RadarArchiver& archiver)
{
  dataHandlerImpl_->archive(archiver);
}

void
DataHandler::wipeArchive(tsr::ARS300 _mode)
{
  dataHandlerImpl_->wipeArchive(_mode);
}

const Sequence&
DataHandler::sequence() const
{
  return dataHandlerImpl_->sequence();
}

size_t
DataHandler::size() const
{
  return dataHandlerImpl_->size();
}

void
DataHandler::clear()
{
  dataHandlerImpl_->clear();
}

DataHandler::DataHandler()
  : dataHandlerImpl_{ std::make_unique<DataHandler::DataHandlerImpl>(*this) }
{
  parseDirectory(
    "/home/ditzel/fast");
  //  parseDirectory("/home/ditzel/slow");
  registerEvents({ SequenceStepEvent, ErrorEvent });
}

void
serialize(const DataHandler& datahandler)
{
  for (auto& seq : datahandler.sequence())
    seq.serializeJSON();
}
