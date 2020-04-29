set(PARSER_SOURCES
    "grammar_helpers.cpp"
    "parser.cpp"
)

set(PARSER_PUBLIC_HEADERS
    "grammar.h"
    "grammar_helpers.h"
    "parser.h"
)

add_library(selscript_parser OBJECT ${PARSER_SOURCES}
)

target_include_directories(
    selscript_parser
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript_parser
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_parser
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    selscript_parser
    PRIVATE
        common
)
