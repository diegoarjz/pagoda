find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

set(PGFRAMES_SRCS
  "platform_window.cpp"
  "window.cpp"
  "main_loop.cpp"
)

add_pagoda_library(
  NAME     pgframes
  SOURCES  ${PGFRAMES_SRCS}
  DEPENDENCIES
    libpagoda
    imgui::imgui
)

add_subdirectory(platform_windows)
#add_subdirectory(graph_editor)
