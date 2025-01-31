cmake_minimum_required(VERSION 3.5.0)
project(
  mywr
  DESCRIPTION "The cross-platform multiarchitectural memhacking library"
  VERSION 1.0.0
  LANGUAGES C CXX)

add_library("mywr" INTERFACE)

# Configure the project.
target_compile_features("mywr" INTERFACE cxx_std_17)
target_include_directories(
  "mywr" INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mywr>
                   $<INSTALL_INTERFACE:include/mywr>)

# Generate .clangd file
add_subdirectory("scripts/clangd")

# Add demo project compilation.
add_subdirectory("demo")
