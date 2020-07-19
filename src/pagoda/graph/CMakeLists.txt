set(GRAPH_SRCS
    "io/ast_interpreter.cpp"
    "io/ast_interpreter.h"
    "io/ast_node.cpp"
    "io/ast_node.h"
    "io/ast_node_visitor.h"
    "io/graph_definition_node.cpp"
    "io/graph_definition_node.h"
    "io/graph_reader_grammar.h"
    "io/graph_reader_grammar_helper.cpp"
    "io/graph_reader_grammar_helper.h"
    "io/graph_statement_node.h"
    "io/named_argument.cpp"
    "io/named_argument.h"
    "io/node_definition_node.cpp"
    "io/node_definition_node.h"
    "io/node_link_node.cpp"
    "io/node_link_node.h"
    "io/parse_result.h"
    "io/reader.cpp"
    "io/reader.h"
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
    "router_node.cpp"
    "router_node.h"
    "scheduler.h"
    "unknown_node_type.cpp"
    "unknown_node_type.h"
    "unsupported_node_link.cpp"
    "unsupported_node_link.h"
)

add_library(graph OBJECT ${GRAPH_SRCS})

target_include_directories(
    graph
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    graph
    PRIVATE
        cxx_std_17
)

target_compile_options(
    graph
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    graph
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    graph
    PRIVATE
        common
)

#########################################################################
# Unit tests
#########################################################################

set(GEOMETRY_UNIT_TESTS
    "graph.test.cpp"
)

foreach(unit_test_src ${GEOMETRY_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
