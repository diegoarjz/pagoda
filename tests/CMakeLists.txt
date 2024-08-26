add_subdirectory(unit_tests)
add_subdirectory(regression_tests)

if (NOT WIN32)
  add_subdirectory(pgscript)
endif()
add_subdirectory(plugin)
