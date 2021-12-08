set(RENDERER_SRCS
  "buffer.cpp"
  "buffer.h"
  "interleaved_buffer.cpp"
  "interleaved_buffer.h"
  "rendering_system.cpp"
  "rendering_system.h"
  "rendering_component.cpp"
  "rendering_component.h"
  "operations/render.cpp"
  "operations/render.h"
  "gl/mesh.cpp"
  "gl/mesh.h"
  "gl/default_material.cpp"
  "gl/default_material.h"
  "gl/render_pass.cpp"
  "gl/render_pass.h"
  "gl/grid_render_task.cpp"
  "gl/grid_render_task.h"
)

add_pagoda_module(
  NAME          renderer
  SOURCES       ${RENDERER_SRCS}
  DEPENDENCIES  libpagoda GLEW::GLEW
)


#########################################################################
# Unit tests
#########################################################################
set(RENDERER_UNIT_TESTS
  "buffer.test.cpp"
)

foreach(unit_test_src ${RENDERER_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
target_link_libraries("buffer_test" PRIVATE libpgeditor)
