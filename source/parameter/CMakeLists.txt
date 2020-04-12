set(PARAMETER_SRCS
    "variable.cpp"
    "variable.h"
    "variable_resolver.cpp"
    "variable_resolver.h"
)

set(PARAMETER_PUBLIC_HEADERS
    "variable.h"
    "variable_resolver.h"
)

add_library(parameter OBJECT ${PARAMETER_SRCS})

target_include_directories(
    parameter
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    parameter
    PRIVATE
        cxx_std_17
)

target_link_libraries(
    parameter
    PUBLIC
        common
)

target_compile_options(
    parameter
    PRIVATE
        -Wall
        -Werror
)

include(GNUInstallDirs)
install(TARGETS parameter
    EXPORT parameter-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/parameter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/parameter
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/parameter)

install(EXPORT parameter-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


