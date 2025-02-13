/**
 * @file protect.inl
 * @author themusaigen
 * @brief Module that implements `protect.hpp` for Windows.
 *
 * This file provides the implementation of the `protect` module for Windows,
 * which includes functions to retrieve and modify memory protection flags.
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * The `protect` module is a part of the mywr library and is responsible for
 * managing memory protection flags on Windows platforms. It provides two main
 * functions: `get_protect` and `set_protect`.
 *
 * The `get_protect` function retrieves the current memory protection flags for
 * a given address. It uses the `VirtualQuery` function to obtain the memory
 * basic information, which includes the protection flags. The protection flags
 * are then converted to the `protection` enum type and returned.
 *
 * The `set_protect` function modifies the memory protection flags for a
 * specified address and size. It uses the `VirtualProtect` function to change
 * the protection flags. The old protection flags are returned as the result of
 * the function.
 *
 * @note
 * This file is an inline implementation file and should be included in the
 * corresponding header file (`protect.hpp`) to provide the necessary
 * functionality.
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
