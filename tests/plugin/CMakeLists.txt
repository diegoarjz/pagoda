add_library(test_plugin SHARED "plugins/test_plugin.cpp")
target_link_libraries(test_plugin PRIVATE libpagoda)
target_compile_features(test_plugin PRIVATE cxx_std_17)
set_target_properties(test_plugin PROPERTIES PREFIX "")

set(TEST_PLUGIN_JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/test_plugin.json")

add_custom_target(
  create_test_plugins_dir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:test_plugin>/plugins
)

add_custom_target(
  copy_test_plugin_json ALL
  COMMAND ${CMAKE_COMMAND} -E copy
    ${TEST_PLUGIN_JSON_SRC}
    $<TARGET_FILE_DIR:test_plugin>/plugins
  DEPENDS
    create_test_plugins_dir
)

add_custom_target(
  copy_test_plugin ALL
  COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:test_plugin>
    $<TARGET_FILE_DIR:test_plugin>/plugins
  DEPENDS
    create_test_plugins_dir
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
  ENVIRONMENT PAGODA_PLUGIN_PATH=$<TARGET_FILE_DIR:test_plugin>/plugins
)
