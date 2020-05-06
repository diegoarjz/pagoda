set(PROCEDURAL_GRAPH_READER_SRCS
    "ast_interpreter.cpp"
    "ast_interpreter.h"
    "ast_node.cpp"
    "ast_node.h"
    "ast_node_visitor.h"
    "graph_definition_node.cpp"
    "graph_definition_node.h"
    "graph_reader_grammar.h"
    "graph_reader_grammar_helper.cpp"
    "graph_reader_grammar_helper.h"
    "graph_statement_node.h"
    "named_argument.cpp"
    "named_argument.h"
    "node_definition_node.cpp"
    "node_definition_node.h"
    "node_link_node.cpp"
    "node_link_node.h"
)

set(PROCEDURAL_GRAPH_READER_PUBLIC_HEADERS
    "ast_interpreter.h"
    "ast_node.h"
    "ast_node_visitor.h"
    "graph_definition_node.h"
    "graph_reader_grammar.h"
    "graph_reader_grammar_helper.h"
    "graph_statement_node.h"
    "named_argument.h"
    "node_definition_node.h"
    "node_link_node.h"
)

add_library(procedural_graph_reader OBJECT ${PROCEDURAL_GRAPH_READER_SRCS})

target_include_directories(
    procedural_graph_reader
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    procedural_graph_reader
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    procedural_graph_reader
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    procedural_graph_reader
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    procedural_graph_reader
    PRIVATE
        common
)
