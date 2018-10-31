#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

template <class>
class task;

template <class R, class... Args>
class task<R(Args...)>
{
  struct concept;

  template <class F>
  struct model;

  std::unique_ptr<concept> _p;

public:
  template <class F>
  task(F&& f)
    : _p(std::make_unique<model<std::decay<F>>>(std::forward<F>(f)))
  {
  }

  R operator()(Args... args)
  {
    return _p->_invoke(std::forward<Args>(args)...);
  }
};

template <class R, class... Args>
struct task<R(Args...)>::concept
{
  virtual ~concept() = default;
  virtual R _invoke(Args&&...) = 0;
};

template <class R, class... Args>
template <class F>
struct task<R(Args...)>::model final : concept
{
  template <class G>
  model(G&& f)
    : _f(std::forward<G>(f))
  {
  }
  R _invoke(Args&&... args) override
  {
    return std::invoke(_f, std::forward<Args>(args)...);
  }
  F _f;
};

// int main() {
//    task<std::unique_ptr<int>()> f = [_p =
//                                          std::make_unique<int>(42)]() mutable
//                                          {
//        return std::move(_p);
//    };
