find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)

########################################
# OpenGL3 Window
########################################

set(OPENGL3_WINDOW_SRCS
  "imgui_impl_glfw.h"
  "imgui_impl_glfw.cpp"
  "imgui_impl_opengl3.h"
  "imgui_impl_opengl3.cpp"
  "opengl3.cpp"
)

set(OPENGL3_WINDOW_DEPENDENCIES
  "imgui::imgui"
  "glfw"
  GLEW::GLEW
)

add_pagoda_plugin(
  NAME          "pg_gl3_window"
  SOURCES       ${OPENGL3_WINDOW_SRCS}
  DEPENDENCIES  ${OPENGL3_WINDOW_DEPENDENCIES}
  REGISTRY      "PlatformWindow"
  FUNCTION      "CreateWindow"
  DESTINATION   "${CMAKE_BINARY_DIR}/src/bin/plugins"
  JSON          "${PROJECT_SOURCE_DIR}/tools/plugin.json"
)


