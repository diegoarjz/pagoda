set(PARSER_SOURCES
    "grammar.h"
    "grammar_helpers.cpp"
    "parser.cpp"
)

add_library(pgscript_parser OBJECT ${PARSER_SOURCES}
)

target_include_directories(
    pgscript_parser
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
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
      ${PAGODA_COMPILE_OPTIONS}
)

target_link_libraries(
    pgscript_parser
    PRIVATE
        common
        dynamic
)
