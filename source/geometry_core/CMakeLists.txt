set(GEOMETRY_CORE_SRCS
    "geometry.h"
    "geometry_builder.h"
    "geometry_exporter.h"
    "geometry_sizes.h"
    "indexed_container.h"
    "scope.cpp"
    "scope.h"
    "split_point_topology.cpp"
    "split_point_topology.h"
)

set(GEOMETRY_CORE_PUBLIC_HEADERS
    "geometry.h"
    "geometry_builder.h"
    "geometry_exporter.h"
    "geometry_sizes.h"
    "indexed_container.h"
    "scope.cpp"
    "scope.h"
    "split_point_topology.cpp"
    "split_point_topology.h"
)

add_library(geometry_core OBJECT ${GEOMETRY_CORE_SRCS})

target_include_directories(
    geometry_core
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    geometry_core
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    geometry_core
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    geometry_core
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    geometry_core
    PRIVATE
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    geometry_core
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS geometry_core
    EXPORT geometry_core-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/geometry_core
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/geometry_core
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_core)

install(EXPORT geometry_core-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


