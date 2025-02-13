include("FetchContent")

FetchContent_Declare(
  "expected"
  GIT_REPOSITORY "https://github.com/TartanLlama/expected.git"
  GIT_TAG "v1.1.0")

FetchContent_MakeAvailable("expected")
