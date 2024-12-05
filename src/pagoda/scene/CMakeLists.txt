set(SCENE_SRCS
  "camera.cpp"
  "lens.cpp"
  "path.cpp"
  "render_target.cpp"
  "scene_graph.cpp"
  "scene_node.cpp"
  "transformation.cpp"
  "geometry.cpp"
  "mesh.cpp"
  "lines.cpp"
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
)

foreach(unit_test_src ${SCENE_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
