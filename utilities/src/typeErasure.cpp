#include "utilities/typeErasure.hpp"

namespace Tesseract {

void
process(const collection_t& document)
{
  for (auto& anything : document)
    process(anything);
}

void
commit(progress_t& progress, Label label)
{
  //  assert(std::size(progress));
  if (!std::size(progress))
    throw Exception{ "Initial progress necessary for commit" };
  progress.emplace_back(label, progress.back().second);
}

std::optional<record_t>
revoke(progress_t& progress)
{
  //  assert(std::size(progress));
  //  throw Exception{ "Not enough elements for revoke" };
  if (std::size(progress) > 1) {
    const auto _record{ progress.back() };
    progress.pop_back();
    return _record; // C++17 NRVO?
  }
  return std::nullopt;
}

collection_t&
current(progress_t& progress)
{
  //  assert(std::size(progress));
  if (std::size(progress))
    return progress.back().second;
  else
    throw Exception{ "progress does not contain any records" };
}

void
store(progress_t& progress, const record_t& record)
{
  progress.emplace_back(std::move(record));
}

uint16_t
count(collection_t c)
{
  return std::size(c);
};
}
