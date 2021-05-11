find_package(Qt6 COMPONENTS Widgets Core OpenGL OpenGLWidgets REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(renderer)
add_subdirectory(gui)

set(PGEDITOR_SOURCES
  "gui/graph/graph_port_connection.h"
  "gui/graph/graph_node.h"
  "gui/graph/new_node_popup.h"
  "gui/graph/graph_port.h"
  $<TARGET_OBJECTS:renderer>
  $<TARGET_OBJECTS:gui>
)

add_pagoda_library(
  NAME          libpgeditor
  SOURCES       ${PGEDITOR_SOURCES}
  DEPENDENCIES  libpagoda GLEW::GLEW Qt6::Widgets Qt6::Core Qt6::OpenGL Qt6::OpenGLWidgets
)

set_target_properties(libpgeditor PROPERTIES AUTOMOC TRUE)
set_target_properties(libpgeditor PROPERTIES POSITION_INDEPENDENT_CODE ON)
