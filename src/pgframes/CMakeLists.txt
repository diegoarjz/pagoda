find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

set(PGFRAMES_SRCS
  "platform_window.cpp"
  "window.cpp"
  "main_loop.cpp"

  "widgets/widget.cpp"
  "widgets/button.cpp"
  "widgets/label.cpp"
  "widgets/color_edit.cpp"
  "widgets/float_edit.cpp"
  "widgets/tree_view.cpp"
  "widgets/tree_view.cpp"

  "widgets/command_list.cpp"
  "widgets/field.cpp"
  "widgets/text_input.cpp"
)

add_pagoda_library(
  NAME     pgframes
  SOURCES  ${PGFRAMES_SRCS}
  DEPENDENCIES
    libpagoda
    imgui::imgui
    magic_enum::magic_enum
    fmt::fmt
    EnTT::EnTT
)

add_subdirectory(platform_windows)
add_subdirectory(graph_editor)
add_subdirectory(viewer)
