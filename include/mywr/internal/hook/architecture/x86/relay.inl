#pragma once

#include <shared/calling_conventions.hpp>
#include <shared/function_traits.hpp>

#include <tuple>

namespace mywr::hook::inner {
template<typename Hook,
         detail::calling_conventions Convention,
         typename Ret,
         typename Args>
struct relay;

template<typename Hook, typename Ret, typename... Args>
struct relay<Hook,
             detail::calling_conventions::Cdecl,
             Ret,
             std::tuple<Args...>> {
  static auto MYWR_CDECL call(Hook* hook, Args... args) -> Ret {
    const auto& callback = hook->callback();
    if (callback) {
      return callback(*hook, args...);
    }

    return hook->call(args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay<Hook,
             detail::calling_conventions::Stdcall,
             Ret,
             std::tuple<Args...>> {
  static auto MYWR_STDCALL call(Hook* hook, Args... args) -> Ret {
    const auto& callback = hook->callback();
    if (callback) {
      return callback(*hook, args...);
    }

    return hook->call(args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay<Hook,
             detail::calling_conventions::Thiscall,
             Ret,
             std::tuple<Args...>> {
  static auto MYWR_STDCALL call(Hook* hook, Args... args) -> Ret {
    const auto& callback = hook->callback();
    if (callback) {
      return callback(*hook, args...);
    }

    return hook->call(args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay<Hook,
             detail::calling_conventions::Fastcall,
             Ret,
             std::tuple<Args...>> {
  struct fastcall_abuse {
    Hook* hook;
  };

  static auto MYWR_FASTCALL call(fastcall_abuse abuse, Args... args) -> Ret {
    const auto& callback = abuse.hook->callback();
    if (callback) {
      return callback(*abuse.hook, args...);
    }

    return abuse.hook->call(args...);
  }
};

template<typename Hook, typename Fun>
using relay_t = relay<Hook,
                      detail::convention_v<Fun>,
                      detail::return_type_t<Fun>,
                      detail::arguments_t<Fun>>;
} // namespace mywr::hook::inner
