set(test_srcs
    "main.cpp"
    "test_utils.cpp"
    "common/profiler.cpp"
    "common/range.cpp"
    "common/const_str.cpp"
    "math_lib/vec_base.cpp"
    "math_lib/matrix_base.cpp"
    "math_lib/line_3.cpp"
    "math_lib/line_segment_3d.cpp"
    "math_lib/plane.cpp"
    "math_lib/projection.cpp"
    "math_lib/orthogonal.cpp"
    "geometry_core/geometry.cpp"
    "geometry_core/geometry_builder.cpp"
    "geometry_core/geometry_exporter.cpp"
    "geometry_core/indexed_container.cpp"
    "geometry_core/geometry_topology.cpp"
    "geometry_core/scope.cpp"
    "geometry_operations/extrusion.cpp"
    "geometry_operations/create_rect.cpp"
    "geometry_operations/triangulate.cpp"
    "parameter/context.cpp"
    "parameter/expression.cpp"
    "procedural_objects/procedural_object.cpp"
    "procedural_objects/procedural_object_interface.cpp"
    "procedural_objects/geometry_system.cpp"
    "procedural_graph/graph.cpp"
    "procedural_graph/node.cpp"
    "procedural_graph/node_set_visitor.cpp"
    "procedural_graph/graph_reader_grammar.cpp"
    "procedural_graph/parameter_node.cpp"
    )

add_executable(unit_tests ${test_srcs})

set_target_properties(
    unit_tests
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    unit_tests
    PRIVATE
        cxx_std_17
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    unit_tests
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    unit_tests
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

target_compile_definitions(
    unit_tests
    PRIVATE
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
)

target_link_libraries(
    unit_tests
    PRIVATE
        selector
        Boost::chrono
        Boost::system
        pthread
        gmock
        gtest
)

add_test(NAME unit_tests COMMAND unit_tests)
