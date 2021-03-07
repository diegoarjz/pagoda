set(MATERIAL_SRCS
    "material.cpp"
    "material.h"
    "material_system.cpp"
    "material_system.h"
    "material_component.cpp"
    "material_component.h"
    "operations/set_material.cpp"
    "operations/set_material.h"
    "operations/set_texture.cpp"
    "operations/set_texture.h"
    "operations/set_shader.cpp"
    "operations/set_shader.cpp"
    "operations/set_material_attribute.cpp"
    "operations/set_material_attribute.h"
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
