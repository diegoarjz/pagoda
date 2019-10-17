set(INTERMEDIATE_SOURCES
    "anonymous_method.cpp"
    "ast.cpp"
    "ast_printer.cpp"
    "class_declaration.cpp"
    "function_declaration.cpp"
    "get_expression.cpp"
    "parameter.cpp"
    "set_expression.cpp"
)

set(INTERMEDIATE_PUBLIC_HEADERS
    "anonymous_method.h"
    "ast.h"
    "ast_printer.h"
    "ast_visitor.h"
    "class_declaration.h"
    "function_declaration.h"
    "get_expression.h"
    "parameter.h"
    "set_expression.h"
)

add_library(intermediate OBJECT ${INTERMEDIATE_SOURCES})

target_include_directories(
    intermediate
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    intermediate
    PRIVATE
        cxx_std_17
)

target_compile_options(
    intermediate
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

include(GNUInstallDirs)
install(TARGETS intermediate
    EXPORT intermediate-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/intermediate
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/intermediate
)
install(FILES ${INTERMEDIATE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/intermediate)

install(EXPORT intermediate-export
    FILE
        IntermediateTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
