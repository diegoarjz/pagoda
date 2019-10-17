set(BINDING_SOURCES
    "argument_value.cpp"
    "cast_to_base_value.cpp"
    "functional.cpp"
)

set(BINDING_PUBLIC_HEADERS
    "argument_value.h"
    "functional.h"
)

add_library(binding OBJECT ${BINDING_SOURCES})

target_include_directories(
    binding
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    binding
    PRIVATE
        cxx_std_17
)

target_compile_options(
    binding
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    binding
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS binding
    EXPORT binding-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/binding
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/binding
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/binding)

install(EXPORT binding-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
