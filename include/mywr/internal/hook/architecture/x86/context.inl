#pragma once

#include <cstdint>

namespace mywr::hook {
struct context {
  uint32_t return_address{};

  uint32_t eax{};
  uint32_t ecx{};
  uint32_t edx{};
  uint32_t ebx{};

  uint32_t esi{};
  uint32_t edi{};

  uint32_t esp{};
  uint32_t ebp{};
};
} // namespace mywr::hook
