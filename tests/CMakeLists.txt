cmake_minimum_required(VERSION 3.14)


# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59 # v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(memwrapper-tests "address_test.cpp" "protect_test.cpp" "llmo_test.cpp")
target_link_libraries(memwrapper-tests gtest gtest_main ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(memwrapper-tests)
