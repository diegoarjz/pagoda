set(BINDING_SOURCES
    "argument_value.cpp"
    "cast_to_base_value.cpp"
    "functional.cpp"
)

set(BINDING_PUBLIC_HEADERS
    "argument_value.h"
    "functional.h"
)

add_library(selscript_binding OBJECT ${BINDING_SOURCES})

target_include_directories(
    selscript_binding
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript_binding
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_binding
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    selscript_binding
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS selscript_binding
    EXPORT selscript_binding-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_binding
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_binding
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/selscript_binding)

install(EXPORT selscript_binding-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
