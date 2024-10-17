/*********************************************************************
 * @file   relay.hpp
 * @brief  Module containing X32 hook relay.
 * 
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_RELAY32_HPP_
#define MYWR_RELAY32_HPP_

namespace mywr {
namespace hook {
namespace impl {
template<typename Ret, typename Hook, typename... Args>
MYWR_FORCEINLINE Ret common_relay(Hook* hook, Args... args) {
  auto& callback = hook->callback();
  if (callback)
    return callback(*hook, args...);
  else
    return hook->call(args...);
}

template<detail::calling_conventions::Enum, typename, typename, typename>
struct relay_generator;

template<typename Hook, typename Ret, typename... Args>
struct relay_generator<detail::calling_conventions::kCdecl,
                       Hook,
                       Ret,
                       std::tuple<Args...>> {
  static Ret MYWR_CDECL relay(Hook* hook, Args... args) {
    return common_relay<Ret>(hook, args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay_generator<detail::calling_conventions::kStdcall,
                       Hook,
                       Ret,
                       std::tuple<Args...>> {
  static Ret MYWR_STDCALL relay(Hook* hook, Args... args) {
    return common_relay<Ret>(hook, args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay_generator<detail::calling_conventions::kThiscall,
                       Hook,
                       Ret,
                       std::tuple<Args...>> {
  static Ret MYWR_STDCALL relay(Hook* hook, Args... args) {
    return common_relay<Ret>(hook, args...);
  }
};

template<typename Hook, typename Ret, typename... Args>
struct relay_generator<detail::calling_conventions::kFastcall,
                       Hook,
                       Ret,
                       std::tuple<Args...>> {
  /**
   * Popular trick to retrive hook object.
   */
  struct trick {
    Hook* hook;
  };

  static Ret MYWR_FASTCALL relay(trick arg, Args... args) {
    return common_relay<Ret>(arg.hook, args...);
  }
};

template<typename Hook, typename Fun>
using relay_t = relay_generator<traits::convention_v<Fun>,
                                Hook,
                                traits::return_type_t<Fun>,
                                traits::arguments_t<Fun>>;
} // namespace impl
} // namespace hook
} // namespace mywr

#endif // !MYWR_RELAY32_HPP_
