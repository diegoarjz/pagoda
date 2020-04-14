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
        -Werror
)

target_link_libraries(
    selscript_parser
    PRIVATE
        common
)

set_target_properties(
    selscript_parser
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

include(GNUInstallDirs)
install(TARGETS selscript_parser
    EXPORT selscript_parser-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_parser
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_parser
)
install(FILES ${PARSER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/selscript_parser)

install(EXPORT selscript_parser-export
    FILE
        ParserTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
