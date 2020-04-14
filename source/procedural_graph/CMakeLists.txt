set(PROCEDURAL_GRAPH_SRCS
    "breadth_first_node_visitor.h"
    "construction_argument_not_found.cpp"
    "construction_argument_not_found.h"
    "default_scheduler.cpp"
    "default_scheduler.h"
    "execution_queue.cpp"
    "execution_queue.h"
    "graph.cpp"
    "graph.h"
    "graph_dot_exporter.cpp"
    "graph_dot_exporter.h"
    "input_interface_node.cpp"
    "input_interface_node.h"
    "node.cpp"
    "node.h"
    "node_factory.cpp"
    "node_factory.h"
    "node_set.h"
    "node_set.cpp"
    "node_set_visitor.h"
    "node_visitor.h"
    "operation_node.cpp"
    "operation_node.h"
    "output_interface_node.cpp"
    "output_interface_node.h"
    "parameter_node.cpp"
    "parameter_node.h"
    "parse_result.h"
    "reader.cpp"
    "reader.h"
    "router_node.cpp"
    "router_node.h"
    "scheduler.h"
    "unknown_node_type.cpp"
    "unknown_node_type.h"
    "unsupported_node_link.cpp"
    "unsupported_node_link.h"
)

set(PROCEDURAL_GRAPH_PUBLIC_HEADERS
    "breadth_first_node_visitor.h"
    "construction_argument_not_found.h"
    "default_scheduler.h"
    "execution_queue.h"
    "graph.h"
    "graph_dot_exporter.h"
    "input_interface_node.h"
    "node.h"
    "node_factory.h"
    "node_set.h"
    "node_set_visitor.h"
    "node_visitor.h"
    "operation_node.h"
    "output_interface_node.h"
    "parameter_node.h"
    "parse_result.h"
    "reader.h"
    "router_node.h"
    "scheduler.h"
    "unknown_node_type.h"
    "unsupported_node_link.h"
)

add_subdirectory(reader)

add_library(procedural_graph OBJECT ${PROCEDURAL_GRAPH_SRCS})

target_include_directories(
    procedural_graph
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    procedural_graph
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    procedural_graph
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    procedural_graph
    PRIVATE
        -Wall
        -Werror
)

target_compile_definitions(
    procedural_graph
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    procedural_graph
    PRIVATE
        common
)

include(GNUInstallDirs)
install(TARGETS procedural_graph
    EXPORT procedural_graph-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/procedural_graph
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/procedural_graph
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_graph)

install(EXPORT procedural_graph-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


