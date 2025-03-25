#pragma once

namespace mywr::hook::constants {
constexpr auto kJumpSize = 5U;

constexpr auto kNopOpcode = 0x90;

constexpr auto kX86JmpMask = 0xFD;

constexpr auto kX86CallOpcode = 0xE8;
constexpr auto kX86JmpOpcode  = 0xE9;
} // namespace mywr::hook::constants
