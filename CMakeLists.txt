cmake_minimum_required(VERSION 3.5.0)

project(mywr LANGUAGES CXX)

set(MYWR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MYWR_ROOT_PROJECT ON)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/
                      ${CMAKE_MODULE_PATH})

option(MYWR_BUILD_TESTS "Build the tests" ${MYWR_ROOT_PROJECT})

add_subdirectory("vendor")
add_subdirectory("include")

if (MYWR_BUILD_TESTS)
  add_subdirectory("tests")
endif()
