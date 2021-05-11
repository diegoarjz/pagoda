set(GUI_SRCS
  "main_window.cpp"
  "main_window.h"
  "graph/edge.cpp"
  "graph/edge.h"
  "graph/new_connection_edge.cpp"
  "graph/new_connection_edge.h"
  "graph/graph_editor_widget.cpp"
  "graph/graph_editor_widget.h"
  "graph/graph_scene.cpp"
  "graph/graph_scene.h"
  "graph/graph_node.cpp"
  "graph/graph_node.h"
  "graph/graph_port.cpp"
  "graph/graph_port.h"
  "graph/graph_in_port.cpp"
  "graph/graph_in_port.h"
  "graph/graph_out_port.cpp"
  "graph/graph_out_port.h"
  "graph/graph_port_connection.cpp"
  "graph/graph_port_connection.h"
  "graph/graph_connector.cpp"
  "graph/graph_connector.h"
  "graph/node_style.h"
  "graph/new_node_popup.cpp"
  "graph/new_node_popup.h"
  "viewer/viewer.cpp"
  "viewer/viewer.h"
)

add_pagoda_module(
  NAME          gui
  SOURCES       ${GUI_SRCS}
  DEPENDENCIES  libpagoda common graph renderer Qt6::Widgets Qt6::Core Qt6::OpenGL Qt6::OpenGLWidgets GLEW::GLEW
)
set_target_properties(gui PROPERTIES POSITION_INDEPENDENT_CODE ON)

#########################################################################
# Unit tests
#########################################################################
set(GUI_UNIT_TESTS
)

foreach(unit_test_src ${GUI_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
