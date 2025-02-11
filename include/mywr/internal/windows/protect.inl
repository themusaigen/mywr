/**
 * @file protect.hpp
 * @author themusaigen
 * @brief Module that implements `protect.hpp` for Windows.
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>

#include <internal/protect.hpp>

namespace mywr::protect {

[[nodiscard]] static auto get_protect(const address& address) -> protection {
  MEMORY_BASIC_INFORMATION mbi{};
  VirtualQuery(address, &mbi, sizeof(mbi));
  return to_protection_constant(mbi.Protect);
}

static auto set_protect(const address& address,
                        protection     new_protect,
                        size_t         size) -> protection {
  DWORD old_protection{};
  VirtualProtect(
      address, size, from_protection_constant(new_protect), &old_protection);
  return to_protection_constant(old_protection);
}
} // namespace mywr::protect
