
# Common
set(SELECTOR_COMMON_SOURCES
    "common/assertions.cpp"
    "common/profiler.cpp"
    "common/pugixml.cpp"
    "common/statistics.cpp"
    "common/version.cpp"
    "common/logger.cpp"
    "common/utils.cpp"
)

set(SELECTOR_COMMON_PUBLIC_HEADERS
    "common/range.h"
    "common/utils.h"
    "common/version.h"
    "common/assertions.h"
    "common/profiler.h"
    "common/statistics.h"
    "common/logger.h"
)

# Math lib
set(SELECTOR_MATH_LIB_PUBLIC_HEADERS
    "math_lib/cross_product.h"
    "math_lib/length.h"
    "math_lib/matrix_arithmetic.h"
    "math_lib/matrix_base.h"
    "math_lib/normalize.h"
    "math_lib/vec_arithmetic.h"
    "math_lib/vec_base.h"
    )

# Geometry core
set(SELECTOR_GEOMCORE_SOURCES
    "geometry_core/scope.cpp"
    )

set(SELECTOR_GEOMCORE_PUBLIC_HEADERS
    "geometry_core/geometry.h"
    "geometry_core/geometry_sizes.h"
    "geometry_core/geometry_builder.h"
    "geometry_core/geometry_exporter.h"
    "geometry_core/geometry_topology.h"
    "geometry_core/indexed_container.h"
    "geometry_core/scope.h"
    )

# Geometry operations
set(SELECTOR_GEOMETRY_OPERATION_PUBLIC_HEADERS
    "geometry_operations/create_rect.h"
    "geometry_operations/extrusion.h"
)

# Parameter
set(SELECTOR_PARAMETER_SOURCES
    "parameter/parameter.cpp"
    "parameter/context.cpp"
    "parameter/expression.cpp"
    "parameter/parameter_exception.cpp"
    )

set(SELECTOR_PARAMETER_PUBLIC_HEADERS
    "parameter/context.h"
    "parameter/expression.h"
    "parameter/parameter.h"
)

# Procedural Objects
set(SELECTOR_PROCEDURAL_OBJECTS_SOURCES
    "procedural_objects/procedural_object.cpp"
    "procedural_objects/procedural_operation.cpp"
    "procedural_objects/geometry_system.cpp"
    "procedural_objects/geometry_component.cpp"
    "procedural_objects/hierarchical_component.cpp"
    "procedural_objects/hierarchical_system.cpp"
    "procedural_objects/extrude_geometry.cpp"
    "procedural_objects/create_rect.cpp"
    "procedural_objects/triangulate_geometry.cpp"
    )

set(SELECTOR_PROCEDURAL_OBJECTS_PUBLIC_HEADERS
    "procedural_objects/geometry_system.h"
    "procedural_objects/procedural_component.h"
    "procedural_objects/procedural_object.h"
    "procedural_objects/procedural_operation.h"
    "procedural_objects/geometry_component.h"
    "procedural_objects/hierarchical_component.h"
    "procedural_objects/extrude_geometry.h"
    "procedural_objects/create_rect.h"
    "procedural_objects/hierarchical_system.h"
    )

# Procedural Graph
set(SELECTOR_PROCEDURAL_GRAPH_HEADERS
    "procedural_graph/graph.h"
    "procedural_graph/node.h"
    "procedural_graph/reader.h"
    "procedural_graph/parse_result.h"
    "procedural_graph/graph_execution_context.h"
    )

set(SELECTOR_PROCEDURAL_GRAPH_SOURCES
    "procedural_graph/graph.cpp"
    "procedural_graph/node.cpp"
    "procedural_graph/reader.cpp"
    "procedural_graph/scheduler.cpp"
    "procedural_graph/graph_execution_context.cpp"
    "procedural_graph/input_interface_node.cpp"
    "procedural_graph/output_interface_node.cpp"
    "procedural_graph/operation_node.cpp"
    "procedural_graph/parameter_node.cpp"
    "procedural_graph/reader/ast_node.cpp"
    "procedural_graph/reader/graph_definition_node.cpp"
    "procedural_graph/reader/named_argument.cpp"
    "procedural_graph/reader/node_definition_node.cpp"
    "procedural_graph/reader/node_link_node.cpp"
    "procedural_graph/reader/graph_reader_grammar_helper.cpp"
    )

set(SELECTOR_PUBLIC_HEADERS
    "${SELECTOR_COMMON_PUBLIC_HEADERS}"
    "${SELECTOR_MATH_LIB_PUBLIC_HEADERS}"
    "${SELECTOR_GEOMCORE_PUBLIC_HEADERS}"
    "${SELECTOR_GEOMETRY_OPERATION_PUBLIC_HEADERS}"
    "${SELECTOR_PROCEDURAL_GRAPH_HEADERS}"
)

set(SELECTOR_SOURCES
    "${SELECTOR_COMMON_SOURCES}"
    "${SELECTOR_GEOMCORE_SOURCES}"
    "${SELECTOR_PARAMETER_SOURCES}"
    "${SELECTOR_PROCEDURAL_OBJECTS_SOURCES}"
    "${SELECTOR_PROCEDURAL_GRAPH_SOURCES}"
   )

add_library(
    selector
    "${SELECTOR_SOURCES}"
)

set_target_properties(
    selector
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_include_directories(
    selector
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    selector
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    selector
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    selector
    PRIVATE
        -DSELECTOR_VERSION=\"${SELECTOR_VERSION}\"
        -DSELECTOR_BUILD_NUMBER=\"${SELECTOR_BUILD_NUMBER}\"
        -DSELECTOR_BUILD_DATE=\"${SELECTOR_BUILD_DATE}\"
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    selector
    PUBLIC
        selscript
)

include(GNUInstallDirs)
install(TARGETS selector
    EXPORT selector-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/
)

install(FILES
    ${SELECTOR_COMMON_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/common/"
)

install(FILES
    ${SELECTOR_MATH_LIB_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/math_lib/"
)

install(FILES
    ${SELECTOR_GEOMCORE_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_core/"
)

install(FILES
    ${SELECTOR_GEOMETRY_OPERATION_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_operations/"
)

install(FILES
    ${SELECTOR_PARAMETER_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/parameter/"
)

install(FILES
    ${SELECTOR_PROCEDURAL_OBJECTS_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_objects/"
)

install(FILES
    ${SELECTOR_PROCEDURAL_GRAPH_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_graph/"
)

install(EXPORT selector-export
    FILE
        SelectorTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)
