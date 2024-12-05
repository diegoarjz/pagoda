add_pagoda_executable(
  NAME        pagoda
  SOURCES     "pagoda.cpp"
  DEPENDENCIES Boost::program_options
)

add_pagoda_executable(
  NAME        pgeditor
  SOURCES     "pgeditor.cpp"
  DEPENDENCIES pgframes Boost::program_options
)
