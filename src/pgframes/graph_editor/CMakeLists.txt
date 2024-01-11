set(PGFrames_Graph_Editor_SRC
  "graph_editor.cpp"
  "graph_editor.h"

  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_canvas.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_canvas.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/crude_json.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/crude_json.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor_api.cpp"
)

add_pagoda_plugin(
  NAME          "metal"
  SOURCES       ${METAL_WINDOW_SRCS}
  DEPENDENCIES  ${METAL_WINDOW_DEPENDENCIES}
  REGISTRY      "WindowFactory"
  FUNCTION      "CreateWindow"
  DESTINATION   "${CMAKE_BINARY_DIR}/alpha/bin/plugins"
  JSON          "${CMAKE_SOURCE_DIR}/third_party/pagoda/tools/plugin.json"
)

