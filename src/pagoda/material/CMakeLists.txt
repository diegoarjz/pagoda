set(MATERIAL_SRCS
    "material.cpp"
    "material.h"
    "material_component.cpp"
    "material_component.h"
    "material_network.cpp"
    "material_network.h"
    "material_network_visitor.h"
    "material_node.cpp"
    "material_node.h"
    "material_node_registry.h"
    "material_node_registry.cpp"
    "material_system.cpp"
    "material_system.h"
    "shader_gen.cpp"
    "shader_gen.h"
    "shader_source.h"
    "shader_source.cpp"
    "operations/default_material.cpp"
    "operations/default_material.h"
    "operations/set_material.cpp"
    "operations/set_material.h"
    "operations/set_material_attribute.cpp"
    "operations/set_material_attribute.h"
    "operations/set_shader.cpp"
    "operations/set_shader.cpp"
    "operations/set_texture.cpp"
    "operations/set_texture.h"

    "nodes/default_vert.cpp"
    "nodes/default_vert.h"
    "nodes/default_frag.cpp"
    "nodes/default_frag.h"
    "nodes/buffer_view.cpp"
    "nodes/buffer_view.h"
    "nodes/uniform_view.h"
    "nodes/uniform_view.cpp"
)

add_pagoda_module(
  NAME          material
  SOURCES       ${MATERIAL_SRCS}
  DEPENDENCIES  common dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(MATERIAL_UNIT_TESTS
  "shader_gen.test.cpp"
)

foreach(unit_test_src ${MATERIAL_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
