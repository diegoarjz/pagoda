set(PARSER_SOURCES
    "grammar_helpers.cpp"
    "parser.cpp"
)

set(PARSER_PUBLIC_HEADERS
    "grammar.h"
    "grammar_helpers.h"
    "parser.h"
)

add_library(pgscript_parser OBJECT ${PARSER_SOURCES}
)

target_include_directories(
    pgscript_parser
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgscript_parser
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgscript_parser
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    pgscript_parser
    PRIVATE
        common
)
