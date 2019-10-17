set(VALUE_SOURCES
    "base_value.cpp"
    "boolean_value.cpp"
    "builtin_functions.cpp"
    "function.cpp"
    "class_value.cpp"
    "float_value.cpp"
    "integer_value.cpp"
    "instance_value.cpp"
    "null_object_value.cpp"
    "string_value.cpp"
    "type_info.cpp"
    "undefined_operator.cpp"
)

set(VALUE_PUBLIC_HEADERS
    "base_value.h"
    "boolean_value.h"
    "builtin_function.h"
    "builtin_functions.h"
    "callable.h"
    "class_value.h"
    "float_value.h"
    "function.h"
    "instance_value.h"
    "integer_value.h"
    "null_object_value.h"
    "string_value.h"
    "type_info.h"
    "undefined_operator.h"
    "value_visitor.h"
)

add_library(selscript_value OBJECT ${VALUE_SOURCES})

target_include_directories(
    selscript_value
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript_value
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_value
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

include(GNUInstallDirs)
install(TARGETS selscript_value
    EXPORT selscript_value-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_value
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_value
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/selscript_value)

install(EXPORT selscript_value-export
    FILE
        ValueTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
