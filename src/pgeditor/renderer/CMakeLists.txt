set(
  RENDERER_SRCS
  "buffer.cpp"
  "buffer.h"
  "collection.cpp"
  "collection.h"
  "interleaved_buffer.cpp"
  "interleaved_buffer.h"
  "material_network.cpp"
  "material_network.h"
  "material_node.cpp"
  "material_node.h"
  #"rendering_system.cpp"
  #"rendering_system.h"
  #"rendering_component.cpp"
  #"rendering_component.h"
  "renderable.cpp"
  "renderable.h"
  "renderer.cpp"
  "renderer.h"
  "render_pass.h"
  "render_pass.cpp"
  "render_state.h"
  "render_state.cpp"
  "types.cpp"
  "types.h"
  "uniform_buffer.cpp"
  "uniform_buffer.h"
  "vertex_attribute.h"

  "operations/render.cpp"
  "operations/render.h"

  #"gl/mesh.cpp"
  #"gl/mesh.h"
  #"gl/default_material.cpp"
  #"gl/default_material.h"
  #"gl/render_pass.cpp"
  #"gl/render_pass.h"
  #"gl/grid_render_task.cpp"
  #"gl/grid_render_task.h"

  "metal/metal_renderer.h"
  "metal/metal_renderer.mm"
  "metal/metal_texture.h"
  "metal/metal_texture.mm"
  "metal/render_pipeline_state_manager.h"
  "metal/render_pipeline_state_manager.mm"
  "metal/shader_gen.cpp"
  "metal/shader_gen.h"
)

add_pagoda_module(
  NAME renderer
  SOURCES ${RENDERER_SRCS}
  DEPENDENCIES libpagoda GLEW::GLEW
)


#########################################################################
# Unit tests
#########################################################################
set(
  RENDERER_UNIT_TESTS
  "buffer.test.cpp"
  "metal/shader_gen.test.cpp"
)

foreach(unit_test_src ${RENDERER_UNIT_TESTS})
  add_unit_test(${unit_test_src})
endforeach()
target_link_libraries("buffer_test" PRIVATE libpgeditor)
target_link_libraries("shader_gen_test" PRIVATE libpgeditor)
