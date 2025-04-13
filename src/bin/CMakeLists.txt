add_pagoda_executable(
  NAME        pagoda
  SOURCES     "pagoda.cpp"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/"
  DEPENDENCIES Boost::program_options
)

add_pagoda_executable(
  NAME        pgeditor
  SOURCES     "pgeditor.cpp"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/"
  DEPENDENCIES pgframes Boost::program_options
)
