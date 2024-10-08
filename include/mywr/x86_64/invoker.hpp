/*********************************************************************
 * @file   invoker.hpp
 * @brief  Module containing utilities to execute functions stored in memory.
 *
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_INVOKER_HPP_
#define MYWR_INVOKER_HPP_

namespace mywr {
namespace invoker {
namespace impl {
template <detail::calling_conventions::Enum Convention>
struct invoker;

template <>
struct invoker<detail::calling_conventions::kCdecl> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(MYWR_CDECL*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};

template <>
struct invoker<detail::calling_conventions::kWin64> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};

template <>
struct invoker<detail::calling_conventions::kSystemV> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};

#if defined(MYWR_WINDOWS)
template <>
struct invoker<detail::calling_conventions::kStdcall> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(MYWR_STDCALL*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};

template <>
struct invoker<detail::calling_conventions::kThiscall> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(MYWR_THISCALL*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};

template <>
struct invoker<detail::calling_conventions::kFastcall> {
  template <typename Ret, typename... Args>
  static MYWR_INLINE Ret invoke(const address& fn, Args... args) {
    return reinterpret_cast<Ret(MYWR_FASTCALL*)(Args...)>(fn.value())(
        std::forward<Args>(args)...);
  }
};
#endif

template <typename, typename>
struct invoke;

template <typename Fun, typename... Args>
struct invoke<Fun, std::tuple<Args...>> {
private:
  using Ret = traits::return_type_t<Fun>;

public:
  Ret operator()(const address& fn, Args... args) {
    return invoker<traits::convention_v<Fun>>::template invoke<Ret, Args...>(
        fn,
        std::forward<Args>(args)...);
  }
};

template <typename Fun>
using apply = invoke<Fun, traits::arguments_t<Fun>>;

} // namespace impl
} // namespace invoker
} // namespace mywr

namespace mywr {
namespace invoker {
template <typename Fun, typename... Args>
MYWR_INLINE traits::return_type_t<Fun> invoke(const address& fn, Args... args) {
  return impl::apply<Fun>()(fn, std::forward<Args>(args)...);
}
} // namespace invoker
} // namespace mywr

#endif // !MYWR_INVOKER_HPP_
