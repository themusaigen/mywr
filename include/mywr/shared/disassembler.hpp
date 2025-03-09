#pragma once

#include <shared/core/architecture.hpp>

#if defined(MYWR_X86) || defined(MYWR_X86_64)
  #include <shared/architecture/x86_64/disassembler.inl>
#endif
