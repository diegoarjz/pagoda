set(test_srcs
    "main.cpp"
    "dynamic_value/base_dynamic_values.cpp"
    "dynamic_value/free_function_callable_body.cpp"
    "dynamic_value/vector.cpp"
    "dynamic_value/dynamic_plane.cpp"
    "dynamic_value/make_free_function.cpp"
    "dynamic_value/make_member_function.cpp"
    "dynamic_value/call_free_function.cpp"
    "dynamic_value/call_member_function.cpp"
    "dynamic_value/cast_to.cpp"
    "dynamic_value/return_from_function.cpp"
    "dynamic_value/dynamic_value_table.cpp"
    "dynamic_value/dynamic_class.cpp"
    "dynamic_value/has_operators.cpp"
    "dynamic_value/operators.cpp"
    "test_utils.cpp"
    "common/profiler.cpp"
    "common/range.cpp"
    "math_lib/bissectrix.cpp"
    "math_lib/line_3d.cpp"
    "math_lib/line_segment_3d.cpp"
    "math_lib/math_utils.cpp"
    "math_lib/plane.cpp"
    "math_lib/projection.cpp"
    "math_lib/orthogonal.cpp"
    "math_lib/intersections.cpp"
    "math_lib/nearest_points.cpp"
    "geometry_core/geometry.cpp"
    "geometry_core/geometry_builder.cpp"
    "geometry_core/geometry_exporter.cpp"
    "geometry_core/indexed_container.cpp"
    "geometry_core/split_point_topology.cpp"
    "geometry_core/scope.cpp"
    "geometry_operations/ear_clipping.cpp"
    "geometry_operations/extrusion.cpp"
    "geometry_operations/create_rect.cpp"
    "geometry_operations/triangulate.cpp"
    "parameter/expression.cpp"
    "parameter/variable.cpp"
    "procedural_objects/procedural_object.cpp"
    "procedural_objects/procedural_object_interface.cpp"
    "procedural_objects/geometry_system.cpp"
    "procedural_objects/procedural_object_predicates.cpp"
    "procedural_graph/graph.cpp"
    "procedural_graph/node.cpp"
    "procedural_graph/node_set_visitor.cpp"
    "procedural_graph/execution_queue.cpp"
    "procedural_graph/node_visitor.cpp"
    "procedural_graph/graph_reader_grammar.cpp"
    "procedural_graph/parameter_node.cpp"
    "procedural_graph/graph_reader_ast.cpp"
    "pgscript/grammar.cpp"
    )

add_executable(unit_tests ${test_srcs})

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
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_include_directories(
    unit_tests
    PUBLIC
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/pagoda>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_definitions(
    unit_tests
    PRIVATE
        -DASSERTS_ENABLED=${Pagoda_ENABLE_ASSERTIONS}
)

target_link_libraries(
    unit_tests
    PRIVATE
        libpagoda
        Boost::chrono
        Boost::system
        Boost::filesystem
        gmock
        gtest
        pthread
)

file(COPY test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME unit_tests COMMAND unit_tests)
