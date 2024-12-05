set(PGFrames_Graph_Editor_SRC
  "graph_delegate.h"
  "pagoda_graph_delegate.cpp"
  "pagoda_graph_delegate.h"
  "graph_editor.cpp"
  "graph_editor.h"
  "graph_properties.cpp"
  "graph_properties.h"

  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_canvas.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_canvas.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/crude_json.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/crude_json.h"
  "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor/imgui_node_editor_api.cpp"

  "${CMAKE_SOURCE_DIR}/third_party/ImGuiFileDialog/ImGuiFileDialog.cpp"
  "${CMAKE_SOURCE_DIR}/third_party/ImGuiFileDialog/ImGuiFileDialog.h"
)

add_pagoda_plugin(
  NAME          "graph_editor"
  SOURCES       ${PGFrames_Graph_Editor_SRC}
  DEPENDENCIES  libpagoda pgframes viewer_node imgui::imgui magic_enum::magic_enum fmt::fmt Boost::filesystem
  REGISTRY      "WindowFactory"
  FUNCTION      "CreateWindow"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/plugins"
  JSON          "${CMAKE_SOURCE_DIR}/tools/plugin.json"
)

target_include_directories(
  graph_editor
  PRIVATE
    "${CMAKE_SOURCE_DIR}/third_party/imgui-node-editor"
    "${CMAKE_SOURCE_DIR}/third_party/ImGuiFileDialog"
)

target_compile_definitions(
  graph_editor
  PRIVATE
    USE_BOOKMARK
)
