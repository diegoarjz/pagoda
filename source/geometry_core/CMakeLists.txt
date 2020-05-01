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
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
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
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    geometry_core
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    geometry_core
    PRIVATE
        common
)
