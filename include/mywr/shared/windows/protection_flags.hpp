/**
 * @file protection_flags.hpp
 * @author themusaigen
 * @brief Implementation of `protection_flags` module for Windows.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_SHARED_WINDOWS_PROTECTION_FLAGS_HPP_
#define MYWR_SHARED_WINDOWS_PROTECTION_FLAGS_HPP_

#include <shared/os.hpp>
#include <shared/osheaders.hpp>
#include <shared/protection_flags.hpp>

#if defined(MYWR_WINDOWS)

namespace mywr::protect {
static auto to_protection_constant(std::uint32_t protect)
    -> protection_flags::Enum {
  switch (protect) {
  case PAGE_READONLY: return protection_flags::Read;
  case PAGE_READWRITE: return protection_flags::ReadWrite;
  case PAGE_EXECUTE: return protection_flags::Execute;
  case PAGE_EXECUTE_READ: return protection_flags::ReadExecute;
  case PAGE_EXECUTE_READWRITE: return protection_flags::ReadWriteExecute;
  default: return protection_flags::None;
  }
}

static auto from_protection_constant(protection_flags::Enum protect)
    -> std::uint32_t {
  switch (std::uint8_t(protect)) {
  case protection_flags::Read: return PAGE_READONLY;
  case protection_flags::Execute: return PAGE_EXECUTE;
  case protection_flags::ReadWrite: return PAGE_READWRITE;
  case protection_flags::ReadExecute: return PAGE_EXECUTE_READ;
  case protection_flags::ReadWriteExecute: return PAGE_EXECUTE_READWRITE;
  default: return 0;
  }
}
} // namespace mywr::protect

#endif

#endif
