#pragma once

#include <list>
#include <functional>

namespace pagoda::common
{
/**
 * Implements a delegate pattern.
 *
 *     const auto fn = [&]() { <do something here> };
 *     Delegate<void> d;
 *     d.AddCallback(fn);
 *     d();   // will execute the callback
 */
template<typename R, typename... Args>
class Delegate {
public:
  /// Callback type
  using Callback_t = std::function<R(Args...)>;
  /// Type that stores the callbacks
  using CallbackContainer_t = std::list<Callback_t>;
  /// Handle to a callback. Use it to remove a callback.
  using CallbackHandle_t = CallbackContainer_t::iterator;

  /**
   * Calling this will call all callbacks with the given args.
   */
  void operator()(Args... args) {
    for (const auto& cb : m_callbacks) {
      cb(args...);
    }
  }

  /**
   * Adds a callback to this delegate.
   * Returns a handle so the callback can be removed.
   */
  CallbackHandle_t AddCallback(const Callback_t& cb) {
    return m_callbacks.insert(m_callbacks.end(), cb);
  }

  /**
   * Removes the callback given the handle.
   */
  void RemoveCallback(const CallbackHandle_t& handler) {
    m_callbacks.erase(handler);
  }

  /**
   * How many callbacks are in this delegate.
   */
  std::size_t NumCallbacks() const {
    return m_callbacks.size();
  }

private:
  CallbackContainer_t m_callbacks;
};
}
