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

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>
#include <shared/protection_flags.hpp>

#if defined(MYWR_WINDOWS)

namespace mywr::protect {
static auto to_protection_constant(std::uint32_t protect) -> protection::Enum {
  switch (protect) {
  case PAGE_NOACCESS: return protection::NoAccess;
  case PAGE_READONLY: return protection::Read;
  case PAGE_READWRITE: return protection::ReadWrite;
  case PAGE_EXECUTE: return protection::Execute;
  case PAGE_EXECUTE_READ: return protection::ReadExecute;
  case PAGE_EXECUTE_READWRITE: return protection::ReadWriteExecute;
  default: return protection::None;
  }
}

static auto from_protection_constant(protection::Enum protect)
    -> std::uint32_t {
  switch (std::uint8_t(protect)) {
  case protection::NoAccess: return PAGE_NOACCESS;
  case protection::Read: return PAGE_READONLY;
  case protection::Execute: return PAGE_EXECUTE;
  case protection::ReadWrite: return PAGE_READWRITE;
  case protection::ReadExecute: return PAGE_EXECUTE_READ;
  case protection::ReadWriteExecute: return PAGE_EXECUTE_READWRITE;
  default: return 0;
  }
}
} // namespace mywr::protect

#endif

#endif
