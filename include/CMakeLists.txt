if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DMYWR_DEBUG)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE hde)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mywr/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>  # <prefix>/include/mywr
  ${CMAKE_CURRENT_SOURCE_DIR}
)
