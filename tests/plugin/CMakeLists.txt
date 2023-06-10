add_library(test_plugin SHARED "plugins/test_plugin.cpp")
target_link_libraries(test_plugin PRIVATE libpagoda)
target_compile_features(test_plugin PRIVATE cxx_std_17)
set_target_properties(test_plugin PROPERTIES PREFIX "")


if (WIN32)
  set(PLUGIN_EXTENSION "dll")
  set(TEST_PLUGIN_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
elseif (APPLE)
  set(PLUGIN_EXTENSION "dylib")
  set(TEST_PLUGIN_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
elseif (UNIX AND NOT APPLE)
  set(PLUGIN_EXTENSION "so")
  set(TEST_PLUGIN_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

set(TEST_PLUGIN_DIR "${TEST_PLUGIN_BASE_DIR}/plugins")
set(TEST_PLUGIN_SRC "${TEST_PLUGIN_BASE_DIR}/test_plugin.${PLUGIN_EXTENSION}")
set(TEST_PLUGIN_DST "${TEST_PLUGIN_DIR}/test_plugin.${PLUGIN_EXTENSION}")
set(TEST_PLUGIN_JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/test_plugin.json")
set(TEST_PLUGIN_JSON_DST "${TEST_PLUGIN_DIR}/test_plugin.json")

message(STATUS "TEST_PLUGIN_DIR: ${TEST_PLUGIN_DIR}")
message(STATUS "TEST_PLUGIN_SRC: ${TEST_PLUGIN_SRC}")
message(STATUS "TEST_PLUGIN_DST: ${TEST_PLUGIN_DST}")
message(STATUS "TEST_PLUGIN_JSON_SRC: ${TEST_PLUGIN_JSON_SRC}")
message(STATUS "TEST_PLUGIN_JSON_DST: ${TEST_PLUGIN_JSON_DST}")

file(MAKE_DIRECTORY "${TEST_PLUGIN_DIR}")
add_custom_command(
  TARGET test_plugin POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    "${TEST_PLUGIN_SRC}"
    "${TEST_PLUGIN_DST}"
)
add_custom_command(
  TARGET test_plugin POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${TEST_PLUGIN_JSON_SRC}"
  "${TEST_PLUGIN_JSON_DST}"
)

target_compile_definitions(
  test_plugin
  PRIVATE
    $<$<PLATFORM_ID:Windows>:WIN_EXPORT>
)

add_pagoda_executable(
  NAME          load_plugin
  SOURCES       "load_plugin.cpp"
  DEPENDENCIES  libpagoda
)

add_test(NAME load_plugin COMMAND load_plugin)
set_tests_properties(
  load_plugin
  PROPERTIES
  ENVIRONMENT PAGODA_PLUGIN_PATH=${TEST_PLUGIN_DIR}
)
