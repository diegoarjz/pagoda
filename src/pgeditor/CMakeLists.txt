add_subdirectory(gui)

find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

set(PGEDITOR_SOURCES
  "gui/graph_editor/graph_port_connection.h"
  "gui/graph_editor/graph_node.h"
  $<TARGET_OBJECTS:gui>
)

add_pagoda_library(
  NAME          libpgeditor
  SOURCES       ${PGEDITOR_SOURCES}
  DEPENDENCIES  libpagoda Qt6::Widgets Qt6::Core
)

set_target_properties(libpgeditor PROPERTIES AUTOMOC TRUE)
set_target_properties(libpgeditor PROPERTIES POSITION_INDEPENDENT_CODE ON)
