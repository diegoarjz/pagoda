set(GUI_SRCS
  "main_window.cpp"
  "main_window.h"
  "graph_editor/graph_editor_widget.cpp"
  "graph_editor/graph_editor_widget.h"
  "graph_editor/graph_scene.cpp"
  "graph_editor/graph_scene.h"
  "graph_editor/graph_node.cpp"
  "graph_editor/graph_node.h"
  "graph_editor/graph_port.cpp"
  "graph_editor/graph_port.h"
  "graph_editor/graph_in_port.cpp"
  "graph_editor/graph_in_port.h"
  "graph_editor/graph_out_port.cpp"
  "graph_editor/graph_out_port.h"
  "graph_editor/graph_port_connection.cpp"
  "graph_editor/graph_port_connection.h"
  "graph_editor/graph_connector.cpp"
  "graph_editor/graph_connector.h"
  "graph_editor/node_style.h"
  "graph_editor/new_node_popup.cpp"
  "graph_editor/new_node_popup.h"
)

add_pagoda_module(
  NAME          gui
  SOURCES       ${GUI_SRCS}
  DEPENDENCIES  libpagoda common graph
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
