add_pagoda_executable(
  NAME        pagoda
  SOURCES     "pagoda.cpp"
)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Widgets Core OpenGL REQUIRED)

add_pagoda_executable(
  NAME          pgeditor
  SOURCES       "pgeditor.cpp"
  DEPENDENCIES  libpgeditor Qt6::Widgets Qt6::Core Qt6::OpenGL
)
