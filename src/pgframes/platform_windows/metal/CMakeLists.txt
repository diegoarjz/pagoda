find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

########################################
# Metal Window
########################################

set(METAL_WINDOW_SRCS
  "imgui_impl_glfw.h"
  "imgui_impl_glfw.cpp"
  "imgui_impl_metal.h"
  "imgui_impl_metal.mm"
  "metal.mm"
  "metal.h"
)

set(METAL_WINDOW_DEPENDENCIES
  "imgui::imgui"
  "glfw::glfw"
  "-framework Metal"
  "-framework QuartzCore"
)

add_pagoda_plugin(
  NAME          "pg_metal_window"
  SOURCES       ${METAL_WINDOW_SRCS}
  DEPENDENCIES  ${METAL_WINDOW_DEPENDENCIES}
  REGISTRY      "PlatformWindow"
  FUNCTION      "CreateWindow"
  DESTINATION   "${CMAKE_BINARY_DIR}/alpha/bin/plugins"
  JSON          "${CMAKE_SOURCE_DIR}/third_party/pagoda/tools/plugin.json"
)

