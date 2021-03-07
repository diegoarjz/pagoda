configure_file(
    "${PROJECT_SOURCE_DIR}/tests/regression_tests/main.cpp.in"
    "${PROJECT_BINARY_DIR}/tests/regression_tests/main.cpp"
)

set(test_srcs
    "${CMAKE_CURRENT_BINARY_DIR}/main.cpp"
)

add_pagoda_executable(
  NAME          regression_tests
  SOURCES       ${test_srcs}
  DEPENDENCIES  ${PAGODA_GTEST_LIBS}
)
add_test(NAME regression_tests COMMAND regression_tests)
