include(FetchContent)

FetchContent_Declare(
  hde
  GIT_REPOSITORY "https://github.com/themusaigen/hde-cmake.git"
  GIT_TAG c2735a0838a07d30f9974918894e020a88c79137
)

FetchContent_Declare(
  xbyak
  GIT_REPOSITORY "https://github.com/herumi/xbyak.git"
  GIT_TAG "v7.20"
)

FetchContent_MakeAvailable(hde xbyak)

if (MYWR_BUILD_TESTS)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59 # v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
endif()
