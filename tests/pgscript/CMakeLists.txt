set(test_srcs
    "main.cpp"
)

add_pagoda_executable(
  NAME          pgscript_tests
  SOURCES       ${test_srcs}
  DEPENDENCIES  ${PAGODA_GTEST_LIBS}
)

if (WIN32)
  file(COPY test_files DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  file(COPY test_files DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
endif()
add_test(NAME pgscript_tests COMMAND pgscript_tests)
