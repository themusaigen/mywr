cmake_minimum_required(VERSION 3.5.0)
project(
  "mywr"
  DESCRIPTION "The cross-platform multiarchitectural memhacking library"
  VERSION 1.162.0
  LANGUAGES C CXX)

#
add_library(mywr INTERFACE)
add_library(mywr::mywr ALIAS mywr)

# Configure version.
configure_file(
  "${PROJECT_SOURCE_DIR}/include/mywr/shared/core/version.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/mywr/shared/core/version.hpp" @ONLY)

# Options
option(MYWR_BUILD_TESTS "Build the mywr`s tests." ON)
option(MYWR_BUILD_DEMO "Build the mywr`s demo." ON)

# Configure the project.
target_compile_features("mywr" INTERFACE cxx_std_17)
target_include_directories(
  "mywr" INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mywr>
                   $<INSTALL_INTERFACE:include/mywr>)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Export compile commands for clangd.
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  # Generate .clangd file
  add_subdirectory("scripts/clangd")

  # Add demo project compilation.
  if(MYWR_BUILD_DEMO)
    add_subdirectory("demo")
  endif()

  if(MYWR_BUILD_TESTS)
    enable_testing()

    # Fetch the `googletest` framework.
    add_subdirectory("third-party/gtest")
    add_subdirectory("tests")
  endif()
endif()
