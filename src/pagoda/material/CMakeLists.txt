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
    "material_system.cpp"
    "material_system.h"
    "operations/set_material.cpp"
    "operations/set_material.h"
    "operations/set_material_attribute.cpp"
    "operations/set_material_attribute.h"
    "operations/set_shader.cpp"
    "operations/set_shader.cpp"
    "operations/set_texture.cpp"
    "operations/set_texture.h"
)

add_pagoda_module(
  NAME          material
  SOURCES       ${MATERIAL_SRCS}
  DEPENDENCIES  common dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(IMAGE_UNIT_TESTS
)
