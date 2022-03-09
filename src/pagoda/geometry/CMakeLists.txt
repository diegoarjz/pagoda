set(GEOMETRY_SRCS
    "core/geometry.h"
    "core/geometry_builder.h"
    "core/geometry_sizes.h"
    "core/indexed_container.h"
    "core/scope.cpp"
    "core/scope.h"
    "core/split_point_topology.cpp"
    "core/split_point_topology.h"
    "core/planar_texture_projection.h"
    "core/planar_texture_projection.cpp"
    "core/polygon.h"
    "io/geometry_exporter.h"
    #"io/geojson_reader.cpp"
    #"io/geojson_reader.h"
    "algorithms/create_box.h"
    "algorithms/create_rect.h"
    "algorithms/create_sphere.h"
    "algorithms/ear_clipping.h"
    "algorithms/explode_to_faces.h"
    "algorithms/extrusion.h"
    "algorithms/extrusion_profile.h"
    "algorithms/face_offset.h"
    "algorithms/plane_splits.h"
    "algorithms/triangulate.h"
    "algorithms/traverse.h"
    "algorithms/face_normal.h"
    "operations/clip_geometry.cpp"
    "operations/clip_geometry.h"
    "operations/create_box.cpp"
    "operations/create_box.h"
    "operations/create_rect.cpp"
    "operations/create_rect.h"
    "operations/create_sphere.cpp"
    "operations/create_sphere.h"
    "operations/export_geometry.cpp"
    "operations/export_geometry.h"
    "operations/extract_faces.cpp"
    "operations/extract_faces.h"
    "operations/extract_scope.cpp"
    "operations/extract_scope.h"
    "operations/extrude_geometry.cpp"
    "operations/extrude_geometry.h"
    "operations/face_offset.cpp"
    "operations/face_offset.h"
    "operations/repeat_split.cpp"
    "operations/repeat_split.h"
    "operations/rotate.cpp"
    "operations/rotate.h"
    "operations/scale.cpp"
    "operations/scale.h"
    "operations/split.cpp"
    "operations/split.h"
    "operations/translate.cpp"
    "operations/translate.h"
    "operations/triangulate_geometry.cpp"
    "operations/triangulate_geometry.h"
    "operations/scope_texture_projection.cpp"
    "operations/scope_texture_projection.h"
    #"operations/read_geojson.cpp"
    #"operations/read_geojson.h"
    "geometry_component.cpp"
    "geometry_component.h"
    "geometry_system.cpp"
    "geometry_system.h"
    "scope_axis_direction_predicate.cpp"
    "scope_axis_direction_predicate.h"
)

find_package(nlohmann_json REQUIRED)

add_pagoda_module(
  NAME          geometry
  SOURCES       ${GEOMETRY_SRCS}
  DEPENDENCIES  common dynamic nlohmann_json
)

target_include_directories(
  geometry
  PRIVATE
    ${nlohmann_json_INCLUDE_DIR}
)

#########################################################################
# Unit tests
#########################################################################

set(GEOMETRY_UNIT_TESTS
    "core/geometry.test.cpp"
    "core/geometry_builder.test.cpp"
    "core/planar_texture_projection.test.cpp"
    "core/polygon.test.cpp"
    "algorithms/traverse.test.cpp"
    #"io/geojson_reader.test.cpp"
)

foreach(unit_test_src ${GEOMETRY_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
