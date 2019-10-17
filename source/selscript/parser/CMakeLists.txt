set(PARSER_SOURCES
    "grammar_helpers.cpp"
    "parser.cpp"
)

set(PARSER_PUBLIC_HEADERS
    "grammar.h"
    "grammar_helpers.h"
    "parser.h"
)

add_library(parser OBJECT ${PARSER_SOURCES}
        $<TARGET_OBJECTS:intermediate>
)

target_include_directories(
    parser
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    parser
    PRIVATE
        cxx_std_17
)

target_compile_options(
    parser
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    parser
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS parser
    EXPORT parser-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/parser
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/parser
)
install(FILES ${PARSER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/parser)

install(EXPORT parser-export
    FILE
        ParserTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
