set(SCENE_SRCS
  "camera.cpp"
  "camera.h"
  "lens.cpp"
  "lens.h"
  "transformation.cpp"
  "transformation.h"
)

add_pagoda_module(
  NAME          scene
  SOURCES       ${SCENE_SRCS}
  DEPENDENCIES  common math
)

#########################################################################
# Unit tests
#########################################################################

set(SCENE_UNIT_TESTS
  "transformation.test.cpp"
)

foreach(unit_test_src ${SCENE_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
