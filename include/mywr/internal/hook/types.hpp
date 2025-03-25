#pragma once

#include <shared/function_traits.hpp>

#include <functional>
#include <tuple>

namespace mywr::hook::types {
template<typename Hook, typename Ret, typename Args>
struct concat_callback_type;

template<typename Hook, typename Ret, typename... Args>
struct concat_callback_type<Hook, Ret, std::tuple<Args...>> {
  using type = std::function<Ret(const Hook&, Args...)>;
};

template<typename Hook, typename Ret, typename Args>
using concat_callback_type_t =
    typename concat_callback_type<Hook, Ret, Args>::type;

template<typename Hook, typename Fun>
using concat_callback_type_by_fun_t =
    concat_callback_type_t<Hook,
                           detail::return_type_t<Fun>,
                           detail::arguments_t<Fun>>;

template<typename Hook, typename Fun>
class callback : public concat_callback_type_by_fun_t<Hook, Fun> {
private:
  using function = concat_callback_type_by_fun_t<Hook, Fun>;

public:
  callback() noexcept = default;

  template<typename F>
  callback(const F& fun) noexcept
      : function(fun) {}

  template<typename Class, typename Ret, typename... Args>
  callback(Class* object, Ret (Class::*method)(const Hook&, Args...)) noexcept
      : function([object, method](const Hook& hook, Args&&... args) -> Ret {
        return (object->*method)(hook, std::forward<Args>(args)...);
      }) {}
};
} // namespace mywr::hook::types
