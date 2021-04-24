set(RENDERER_SRCS
  "rendering_system.cpp"
  "rendering_system.h"
  "rendering_component.cpp"
  "rendering_component.h"
)

add_pagoda_module(
  NAME          renderer
  SOURCES       ${RENDERER_SRCS}
  DEPENDENCIES  libpagoda
)


#########################################################################
# Unit tests
#########################################################################
set(RENDERER_UNIT_TESTS
)

foreach(unit_test_src ${RENDERER_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
