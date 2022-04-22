set(SCENE_SRCS
  "camera.cpp"
  "camera.h"
  "lens.cpp"
  "lens.h"
  "path.cpp"
  "path.h"
  "scene_graph.h"
  "scene_graph.cpp"
  "scene_node.h"
  "scene_node.cpp"
  "transformation.cpp"
  "transformation.h"

  "nodes/geometry.cpp"
  "nodes/geometry.h"
  "nodes/transform.cpp"
  "nodes/transform.h"
)

add_pagoda_module(
  NAME          scene
  SOURCES       ${SCENE_SRCS}
  DEPENDENCIES  common math EnTT::EnTT
)

#########################################################################
# Unit tests
#########################################################################

set(SCENE_UNIT_TESTS
  "transformation.test.cpp"
  "scene_graph.test.cpp"
  "path.test.cpp"

  "nodes/transform.test.cpp"
)

foreach(unit_test_src ${SCENE_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
