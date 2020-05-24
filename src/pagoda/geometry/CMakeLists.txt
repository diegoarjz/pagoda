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
    "io/geometry_exporter.h"
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
    "geometry_component.cpp"
    "geometry_component.h"
    "geometry_system.cpp"
    "geometry_system.h"
    "scope_axis_direction_predicate.cpp"
    "scope_axis_direction_predicate.h"
)

add_library(geometry OBJECT ${GEOMETRY_SRCS})

target_include_directories(
    geometry
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    geometry
    PRIVATE
        cxx_std_17
)

target_compile_options(
    geometry
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    geometry
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    geometry
    PRIVATE
        common
)

#########################################################################
# Unit tests
#########################################################################

set(GEOMETRY_UNIT_TESTS
    "core/planar_texture_projection.test.cpp"
)

foreach(unit_test_src ${GEOMETRY_UNIT_TESTS})
    get_filename_component(unit_test_base_name ${unit_test_src} NAME)
    string(REPLACE ".test.cpp" "_test" test_name ${unit_test_base_name})
    message(STATUS "src: ${unit_test_src} name: ${test_name}")

    add_executable(${test_name} ${unit_test_src})

    target_include_directories(
        ${test_name}
        PRIVATE
            $<INSTALL_INTERFACE:pagoda>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
            ${Boost_INCLUDE_DIRS}
    )

    target_compile_features(
        ${test_name}
        PRIVATE
            cxx_std_17
    )

    target_compile_options(
        ${test_name}
        PRIVATE
            -Wall
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
            $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
    )

    target_compile_definitions(
        ${test_name}
        PRIVATE
            $<$<CONFIG:DEBUG>:DEBUG>
    )

    target_link_libraries(
        ${test_name}
        PRIVATE
            libpagoda
            gmock
            gtest
            pthread
    )

add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
