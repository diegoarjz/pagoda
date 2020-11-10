set(PGSCRIPT_SOURCES
    "ir/anonymous_method.cpp"
    "ir/ast.cpp"
    "ir/ast_printer.cpp"
    "ir/class_declaration.cpp"
    "ir/function_declaration.cpp"
    "ir/get_expression.cpp"
    "ir/parameter.cpp"
    "ir/set_expression.cpp"
    "interpreter/interpreter.cpp"
    "interpreter/interpreter_visitor.cpp"
    "parser/grammar.h"
    "parser/grammar_helpers.cpp"
    "parser/parser.cpp"
    "value/builtin_functions.cpp"
    "value/builtin_functions.h"
    "value/script_callable_body.h"
    "value/script_callable_body.cpp"
)

add_library(pgscript OBJECT
    ${PGSCRIPT_SOURCES}
)

target_include_directories(
    pgscript
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgscript
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgscript
    PRIVATE
    #-Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    pgscript
    PRIVATE
        common
)
