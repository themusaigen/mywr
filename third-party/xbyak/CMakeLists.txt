include(FetchContent)

FetchContent_Declare(
  "xbyak"
  GIT_REPOSITORY "https://github.com/herumi/xbyak.git"
  GIT_TAG "v7.24.2")

FetchContent_MakeAvailable(xbyak)
