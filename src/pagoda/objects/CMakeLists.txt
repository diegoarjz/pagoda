set(OBJECTS_SRCS
    "operations/router.cpp"
    "operations/router.h"
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
    "procedural_object_predicate.cpp"
    "procedural_object_predicate.h"
    "procedural_object_predicate_registry.cpp"
    "procedural_object_predicate_registry.h"
    "procedural_object_system.cpp"
    "procedural_object_system.h"
    "procedural_operation.cpp"
    "procedural_operation.h"
    "unknown_operation.cpp"
    "unknown_operation.h"
)

add_library(objects OBJECT ${OBJECTS_SRCS})

target_include_directories(
    objects
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    objects
    PRIVATE
        cxx_std_17
)

target_compile_options(
    objects
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    objects
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    objects
    PRIVATE
        common
)

#########################################################################
# Unit tests
#########################################################################

set(GEOMETRY_UNIT_TESTS
    "procedural_object.test.cpp"
    "procedural_operation.test.cpp"
)

foreach(unit_test_src ${GEOMETRY_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
