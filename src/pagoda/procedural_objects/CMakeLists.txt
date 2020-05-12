set(PROCEDURAL_OBJECTS_SRCS
    "geometry_component.cpp"
    "geometry_component.h"
    "geometry_system.cpp"
    "geometry_system.h"
    "hierarchical_component.cpp"
    "hierarchical_component.h"
    "hierarchical_system.cpp"
    "hierarchical_system.h"
    "operation_factory.cpp"
    "operation_factory.h"
    "procedural_component.cpp"
    "procedural_component.h"
    "procedural_component_system.h"
    "procedural_component_system_base.cpp"
    "procedural_component_system_base.h"
    "procedural_object.cpp"
    "procedural_object.h"
    "procedural_object_mask.h"
    "procedural_object_predicate.cpp"
    "procedural_object_predicate.h"
    "procedural_object_predicate_registry.cpp"
    "procedural_object_predicate_registry.h"
    "procedural_object_system.cpp"
    "procedural_object_system.h"
    "procedural_operation.cpp"
    "procedural_operation.h"
    "procedural_operation_object_interface.cpp"
    "procedural_operation_object_interface.h"
    "scope_axis_direction_predicate.cpp"
    "scope_axis_direction_predicate.h"
    "unknown_operation.cpp"
    "unknown_operation.h"
)

add_library(procedural_objects OBJECT ${PROCEDURAL_OBJECTS_SRCS})

target_include_directories(
    procedural_objects
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    procedural_objects
    PRIVATE
        cxx_std_17
)

target_compile_options(
    procedural_objects
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    procedural_objects
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    procedural_objects
    PRIVATE
        common
)
