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

add_pagoda_module(
  NAME        pgscript
  SOURCES     ${PGSCRIPT_SOURCES}
  DEPENDENCIES  common dynamic
)
