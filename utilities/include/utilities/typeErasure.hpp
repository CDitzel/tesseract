#pragma once
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <utilities/annotations.hpp>
#include <utilities/subdirectoryHandler.hpp>
#include <vector>

namespace Tesseract {

template <typename T>
void process(const T& anything) // fall back option
{ // When a nontemplate function provides an equally good match for a call
  // ithen a function template, the nontemplate version is preferred
  std::cout << anything << std::endl;
}

class object_t
{
public:
  object_t() = default;

  //  C++ allows a non-template class to have a templated constructor
  template <typename T>
  object_t(T x) // T models drawable
    : self_(std::make_shared<model_t<T>>(std::move(x)))
  {
  }
  // forward the call to the pointee
  friend void process(const object_t& x) { x.self_->process_(); }

private: // nested private interface
  struct concept_t
  {
    virtual ~concept_t() = default;
    virtual void process_() const = 0;
  };

  template <typename T>
  struct model_t final : concept_t
  {
    explicit model_t(T&& anything)
      : data_(std::forward<T>(anything))
    {
    }
    void process_() const override { process(data_); }
    T data_;
  };

public:
  template <typename T>
  T value() const
  { // no need to dynamic-cast here
    return static_cast<const model_t<T>*>(self_.get())->data_;
  }

private:
  std::shared_ptr<const concept_t> self_;
};

void process(const object_t& x);

using collection_t = std::vector<object_t>;
using record_t = std::pair<Label, collection_t>;
using progress_t = std::vector<record_t>;

void process(const collection_t&);

[[nodiscard]] uint16_t count(collection_t c);

void commit(progress_t&, Label);
[[nodiscard]] std::optional<record_t> revoke(progress_t&);
void store(progress_t&, const record_t& d);
[[nodiscard]] collection_t& current(progress_t&);

template <typename... Ts>
void
wipe(Ts&... ts)
{
  //  (ts.erase(std::next(std::cbegin(ts)), std::cend(ts)), ...);
  (ts.assign({ record_t{} }), ...);
}
}
