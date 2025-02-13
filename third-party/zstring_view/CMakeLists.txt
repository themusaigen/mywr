include(FetchContent)

FetchContent_Declare(
  "zstring_view"
  GIT_REPOSITORY "https://github.com/moonpotato/zstring_view.git")

FetchContent_MakeAvailable(zstring_view)

add_library(zstring_view INTERFACE)

target_include_directories(zstring_view INTERFACE "${zstring_view_SOURCE_DIR}")
