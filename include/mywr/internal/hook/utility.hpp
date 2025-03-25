#pragma once

#include <shared/core/address.hpp>
#include <shared/disassembler.hpp>

#include <cstddef>

namespace mywr::hook::utility {
[[nodiscard]] inline auto get_at_least_n_bytes(const address& code,
                                               size_t minimal_bytes) noexcept {
  using mywr::disassembler::disassembler;

  disassembler disasm;
  size_t       size{};

  while (size < minimal_bytes) {
    size += disasm.disassemble(code.value() + size).length();
  }

  return size;
}
} // namespace mywr::hook::utility
