/*********************************************************************
 * @file   detail.hpp
 * @brief  Utility-module contains internal implementation of other modules or
 * utils
 *
 * @author themusaigen
 * @date   September 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_DETAIL_HPP_
#define MYWR_DETAIL_HPP_

namespace mywr {
/**
 * @brief Namespace that contains utilities for other modules or user.
 */
namespace detail {
/**
 * @brief Calculates the relative address.
 *
 * @details
 * It is mainly used to create hooks based on unconditional jump (jmp) or call
 * opcode.
 *
 * @code{.cpp}
 * auto rip = mywr::detail::compute_relative_address(MyHookerFunction,
 * HookAddress);
 * @endcode
 *
 * @param[in] dest  The address in memory where the relative address should be
 * calculated.
 * @param[in] src   The address in memory from where you need to calculate the
 * relative address.
 * @param[in] oplen The size of the instruction.
 *
 * @return Relative address.
 */
MYWR_FORCEINLINE address
compute_relative_address(const address& dest,
                         const address& src,
                         const std::size_t oplen = 5u) {
  return dest.value() - src.value() - oplen;
}

/**
 * @brief Restores an absolute address through a relative one.
 *
 * @details
 * It is mainly used to overwrite relative addresses in the trampoline when
 * creating a hook.
 *
 * @code{.cpp}
 * auto abs = mywr::detail::restore_absolute_address(rip, HookAddress);
 * @endcode
 *
 * @param[in] ip    The relative address.
 * @param[in] src   The address of the beginning of the instruction.
 * @param[in] oplen The size of the instruction.
 *
 * @return Absolute address.
 */
MYWR_FORCEINLINE address
restore_absolute_address(const address& ip,
                         const address& src,
                         const std::size_t oplen = 5u) {
  return ip.value() + src.value() + oplen;
}

/**
 * @brief Performs forced cast to an arbitary pointer.
 *
 * @details
 * Mostly used to force cast member function pointer to arbitary pointer.
 */
template <typename Out, typename In>
constexpr Out force_cast(In in) {
  Out out;
  ::memcpy(&out, &in, sizeof(Out));
  return out;
}
} // namespace detail
} // namespace mywr

#endif // !MYWR_DETAIL_HPP_
