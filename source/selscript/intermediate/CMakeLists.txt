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

add_library(selscript_intermediate OBJECT ${INTERMEDIATE_SOURCES})

target_include_directories(
    selscript_intermediate
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript_intermediate
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_intermediate
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

include(GNUInstallDirs)
install(TARGETS selscript_intermediate
    EXPORT selscript_intermediate-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_intermediate
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_intermediate
)
install(FILES ${INTERMEDIATE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/selscript_intermediate)

install(EXPORT selscript_intermediate-export
    FILE
        IntermediateTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
