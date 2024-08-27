add_subdirectory(regression_tests)
add_subdirectory(plugin)

if (NOT WIN32)
  add_subdirectory(unit_tests)
  add_subdirectory(pgscript)
endif()
