#pragma once

#include <cstdint>

namespace mywr::hook {
enum class hook_error : std::uint8_t {
  AlreadyInstalledError,
  AlreadyRemovedError,
  NotExecutableError,
  ProtectViolationError,
  NotEnoughSpaceError,
  InvalidAddressError,
  BackupCreatingError,
  BackupRestoringError,
  AllocateCodecaveError,
  DeallocateCodecaveError,
  WriteMemoryError,
  UsercodeJumpRemoveError,
  ReinstallHookError
};
} // namespace mywr::hook
