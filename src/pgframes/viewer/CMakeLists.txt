find_package(glew REQUIRED)

set(PGFrames_Viewer_Window_SRC
  "viewer_window.cpp"
  "viewer_window.h"

  "viewer_camera.cpp"

  "renderer/renderer.cpp"
  "renderer/gl_renderer.cpp"
  "renderer/render_surface.cpp"
  "renderer/renderable.cpp"

  "renderer/gl/mesh.cpp"
  "renderer/gl/shader_program.cpp"
  "renderer/gl/frame_buffer.cpp"
)

add_pagoda_plugin(
  NAME          "viewer_window"
  SOURCES       ${PGFrames_Viewer_Window_SRC}
  DEPENDENCIES  libpagoda pgframes imgui::imgui magic_enum::magic_enum fmt::fmt Boost::filesystem GLEW::GLEW glfw
  REGISTRY      "WindowFactory"
  FUNCTION      "CreateWindow"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/plugins"
  JSON          "${PROJECT_SOURCE_DIR}/tools/plugin.json"
)

set(PGFrames_Viewer_Node_SRC
  "viewer_node.cpp"
  "viewer_node.h"
)

add_pagoda_plugin(
  NAME          "viewer_node"
  SOURCES       ${PGFrames_Viewer_Node_SRC}
  DEPENDENCIES  libpagoda pgframes imgui::imgui magic_enum::magic_enum fmt::fmt Boost::filesystem viewer_window
  REGISTRY      "Node"
  FUNCTION      "CreateNode"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/plugins"
  JSON          "${PROJECT_SOURCE_DIR}/tools/plugin.json"
)
