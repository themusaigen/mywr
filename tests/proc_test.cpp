#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

#if defined(MYWR_UNIX)
using namespace mywr::proc;

std::vector<memory_region> regions;

TEST(ProcTest, ProcessesParsing) {
  ASSERT_NO_THROW(parse_maps(regions));
  ASSERT_FALSE(regions.empty());
}

TEST(ProcTest, HandlesRegionBeginEnd) {
  for (auto& region : regions) {
    ASSERT_GT(region.begin, 0);
    ASSERT_GT(region.end, 0);
  }
}

TEST(ProcTest, HandlesPermissions) {
  for (auto& region : regions) {
    /**
     * By default, `PROT_READ` will be anyways.
     */
    ASSERT_GE(region.permissions, PROT_READ);
  }
}

TEST(ProcTest, HandlesPrivateShared) {
  for (auto& region : regions) {
    /**
     * Region must be shared or private.
     */
    if (!region.is_shared) {
      ASSERT_TRUE(region.is_private);
    }
  }
}

/**
 * Offset, device minor, major, inode, pathname can be empty (or zero), so we don't test
 * them.
 */

#endif
