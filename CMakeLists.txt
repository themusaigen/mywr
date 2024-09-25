cmake_minimum_required(VERSION 3.5.0)

project(mywr LANGUAGES CXX)

set(MYWR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MYWR_ROOT_PROJECT ON)
endif()

option(MYWR_BUILD_TESTS "Build the tests" ${MYWR_ROOT_PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>  # <prefix>/include/mywr
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if (MYWR_BUILD_TESTS)
  add_subdirectory(tests)
endif()
