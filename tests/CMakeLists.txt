# CMakeLists.txt for mywr project tests

# Add an executable target named "mywr_tests"
add_executable("mywr_tests")

# Define a list of source files for the test target
set(MYWR_TEST_SOURCES
    "shared/address.cpp"
    "shared/disassembler.cpp"
    "internal/protect.cpp"
    "internal/llmo.cpp"
    "internal/patch.cpp"
    "internal/scanner.cpp"
    "internal/allocator.cpp")

# Check the size of void pointer to determine the architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # If the architecture is 64-bit, append the 64-bit invoker source file to the
  # list
  list(APPEND MYWR_TEST_SOURCES "internal/x64/invoker64.cpp")
else()
  # If the architecture is 32-bit, append the 32-bit invoker source file to the
  # list
  list(APPEND MYWR_TEST_SOURCES "internal/x32/invoker32.cpp")
endif()

# Set the C++ standard for the target to C++17
target_compile_features(mywr_tests PRIVATE cxx_std_17)

# Link the "mywr_tests" target with the "mywr" library and the GoogleTest
# library
target_link_libraries(mywr_tests PRIVATE mywr GTest::gtest_main)

# Set the source files for the "mywr_tests" target
target_sources(mywr_tests PRIVATE ${MYWR_TEST_SOURCES})

# Include the GoogleTest module for discovering tests
include(GoogleTest)

# Discover and run the tests for the "mywr_tests" target
gtest_discover_tests(mywr_tests)
