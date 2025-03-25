/**
 * @file protection_flags.inl
 * @author themusaigen
 * @brief Implementation of `protection_flags` module for Windows.
 *
 * @copyright Copyright (c) 2025
 *
 * This file provides the platform-specific implementation of the
 * `protection_flags` module for Windows. It includes the
 * `to_protection_constant` and `from_protection_constant` functions, which
 * convert between system-specific protection constants and the `mywr`
 * protection constant.
 *
 * The `to_protection_constant` function maps Windows protection constants to
 * the corresponding `mywr` protection constants. It uses a switch statement to
 * handle different Windows protection constants and returns the corresponding
 * `mywr` protection constant. If the input protection constant is not
 * recognized, it returns `protection::None`.
 *
 * The `from_protection_constant` function maps `mywr` protection constants to
 * the corresponding Windows protection constants. It uses a switch statement to
 * handle different `mywr` protection constants and returns the corresponding
 * Windows protection constant. If the input protection constant is not
 * recognized, it returns 0.
 *
 * Note: This implementation assumes that the necessary Windows headers are
 * included in the `shared/core/osheaders.hpp` file.
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>
#include <shared/protection_flags.hpp>

namespace mywr::protect {
static auto to_protection_constant(uint32_t protect) -> protection {
  switch (protect) {
  case PAGE_NOACCESS: return protection::NoAccess;
  case PAGE_READONLY: return protection::Read;
  case PAGE_WRITECOPY: return protection::WriteCopy;
  case PAGE_READWRITE: return protection::ReadWrite;
  case PAGE_EXECUTE: return protection::Execute;
  case PAGE_EXECUTE_READ: return protection::ReadExecute;
  case PAGE_EXECUTE_READWRITE: return protection::ReadWriteExecute;
  case PAGE_EXECUTE_WRITECOPY: return protection::ReadWriteCopyExecute;
  default: return protection::None;
  }
}

static auto from_protection_constant(protection protect) -> uint32_t {
  switch (protect) {
  case protection::NoAccess: return PAGE_NOACCESS;
  case protection::Read: return PAGE_READONLY;
  case protection::WriteCopy: return PAGE_WRITECOPY;
  case protection::Execute: return PAGE_EXECUTE;
  case protection::ReadWrite: return PAGE_READWRITE;
  case protection::ReadExecute: return PAGE_EXECUTE_READ;
  case protection::ReadWriteExecute: return PAGE_EXECUTE_READWRITE;
  case protection::ReadWriteCopyExecute: return PAGE_EXECUTE_WRITECOPY;
  case protection::Write:
  case protection::Copy:
  case protection::None:
  default: return 0;
  }
}
} // namespace mywr::protect
