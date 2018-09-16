#ifndef COMMON_H__
#define COMMON_H__

#include <type_traits>
#include <memory>

#include "rc.h"

namespace {
  template <typename T>
  constexpr auto to_underlying(T t) noexcept {
    return static_cast<std::underlying_type_t<T>>(t);
  }

  template<typename T>
  using Pimp = const std::unique_ptr<T>;
}

namespace hal {
  struct GBoard;

  struct Exception : std::runtime_error {
    RC_t m_rc;

    Exception(const std::string& msg, RC_t rc = RC_FAILURE)
      : std::runtime_error(msg), m_rc(rc) {}

    RC_t GetReason() { return m_rc; }
  };
}

#endif /* COMMON_H__ */
