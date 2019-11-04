set(VALUE_SOURCES
    "builtin_functions.cpp"
    "builtin_functions.h"
    "script_callable_body.h"
    "script_callable_body.cpp"
)

set(VALUE_PUBLIC_HEADERS
    "builtin_functions.h"
    "script_callable_body.h"
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
