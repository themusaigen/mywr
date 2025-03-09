include(FetchContent)

FetchContent_Declare(
  "zydis"
  GIT_REPOSITORY "https://github.com/zyantific/zydis.git"
  GIT_TAG "v4.1.1")

FetchContent_MakeAvailable(zydis)
