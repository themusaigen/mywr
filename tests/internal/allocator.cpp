#include <gtest/gtest.h>

#include <shared/sys.hpp>
#include <shared/error_handling.hpp>

#include <internal/allocator.hpp>
#include <internal/memmap.hpp>

using namespace mywr::sys;
using namespace mywr::allocator;
using namespace mywr::memmap;
using namespace mywr::error_handling;

TEST(Allocator, OnlySizeSpecified) {
  const auto result{allocate(get_page_size())};

  EXPECT_TRUE(result.has_value())
      << "Failed to allocate memory, got error: " << result.error();
  EXPECT_TRUE(is_memory_page_occupied(result.value()));

  if (result.has_value()) {
    EXPECT_TRUE(deallocate(result.value()))
        << "Failed to deallocate memory, got error: " << get_last_error();
    EXPECT_TRUE(is_memory_page_free(result.value()));
  }
}

TEST(Allocator, Raii) {
  mywr::address_t address{};

  {
    scoped_memory_block block(get_page_size());

    EXPECT_TRUE(block.allocated())
        << "Failed to allocate memory, got error: " << block.error();
    EXPECT_TRUE(is_memory_page_occupied(block.get()));

    address = block.get().value();
  }

  EXPECT_TRUE(is_memory_page_free(address))
      << "Failed to deallocate memory at address (decimal): " << address;
}
