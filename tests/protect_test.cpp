#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

#include <map>

namespace protect = mywr::protect;

using protect::memory_prot;

TEST(ProtectTest, ShouldConvertProtectionConstants) {
#if defined(MYWR_WINDOWS)
  std::map<uint32_t, memory_prot::Enum> protect = {
      {PAGE_NOACCESS,          memory_prot::kNoAccess        },
      {PAGE_READONLY,          memory_prot::kReadOnly        },
      {PAGE_READWRITE,         memory_prot::kReadWrite       },
      {PAGE_WRITECOPY,         memory_prot::kWriteCopy       },
      {PAGE_EXECUTE,           memory_prot::kExecute         },
      {PAGE_EXECUTE_READ,      memory_prot::kExecuteRead     },
      {PAGE_EXECUTE_READWRITE, memory_prot::kExecuteReadWrite},
      {PAGE_EXECUTE_WRITECOPY, memory_prot::kExecuteWriteCopy},
  };
#elif defined(MYWR_UNIX)
  std::map<uint32_t, memory_prot::Enum> protect = {
      {PROT_NONE,                          memory_prot::kNoAccess        },
      {PROT_READ,                          memory_prot::kRead            },
      {PROT_READ | PROT_WRITE,             memory_prot::kReadWrite       },
      {PROT_EXEC,                          memory_prot::kExecute         },
      {PROT_EXEC | PROT_READ,              memory_prot::kExecuteRead     },
      {PROT_EXEC | PROT_READ | PROT_WRITE, memory_prot::kExecuteReadWrite}
  };
#endif

  // Implicit test.
  for (const auto& [value, kind] : protect) {
    ASSERT_EQ(protect::to_protection_constant(value), kind);
    ASSERT_EQ(protect::from_protection_constant(kind), value);
  }
}

TEST(ProtectTest, ShouldQueryProtection) {
  int value = 0;

  ASSERT_EQ(protect::get_protect(&value), memory_prot::kReadWrite);
}

TEST(ProtectTest, ShouldChangeProtection) {
  int value = 2;

  ASSERT_NE(protect::get_protect(&value), memory_prot::kExecuteReadWrite);
  ASSERT_EQ(protect::set_protect(
                &value, sizeof(value), memory_prot::kExecuteReadWrite),
            memory_prot::kReadWrite);
  ASSERT_EQ(protect::get_protect(&value), memory_prot::kExecuteReadWrite);
  ASSERT_EQ(
      protect::set_protect(&value, sizeof(value), memory_prot::kReadWrite),
      memory_prot::kExecuteReadWrite);
}

TEST(ProtectTest, ShouldUseRAII) {
  int value = 2;

  ASSERT_EQ(protect::get_protect(&value), memory_prot::kReadWrite);

  {
    protect::scoped_protect protect{
        &value, sizeof(value), memory_prot::kExecuteReadWrite};

    ASSERT_EQ(protect::get_protect(&value), memory_prot::kExecuteReadWrite);
  }

  ASSERT_EQ(protect::get_protect(&value), memory_prot::kReadWrite);
}
