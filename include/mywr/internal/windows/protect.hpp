/**
 * @file protect.hpp
 * @author themusaigen
 * @brief Module that implements `protect.hpp` for Windows.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_INTERNAL_WINDOWS_PROTECT_HPP_
#define MYWR_INTERNAL_WINDOWS_PROTECT_HPP_

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>

#include <internal/protect.hpp>

#if defined(MYWR_WINDOWS)
namespace mywr::protect {

[[nodiscard]] static auto get_protect(const mywr::address& address)
    -> protection::Enum {
  MEMORY_BASIC_INFORMATION mbi{};
  VirtualQuery(address, &mbi, sizeof(mbi));
  return to_protection_constant(mbi.Protect);
}

static auto set_protect(const mywr::address& address,
                        protection::Enum     new_protect,
                        std::size_t          size) -> protection::Enum {
  DWORD old_protection{};
  VirtualProtect(
      address, size, from_protection_constant(new_protect), &old_protection);
  return to_protection_constant(old_protection);
}
} // namespace mywr::protect
#endif

#endif
