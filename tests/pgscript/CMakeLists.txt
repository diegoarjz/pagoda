set(test_srcs
    "main.cpp"
)

add_pagoda_executable(
  NAME          pgscript_tests
  SOURCES       ${test_srcs}
  DEPENDENCIES  ${PAGODA_GTEST_LIBS}
)

file(COPY test_files DESTINATION "${CMAKE_BINARY_DIR}/bin")
add_test(NAME pgscript_tests COMMAND pgscript_tests)
