set(OBJECTS_SRCS
    "operations/router.cpp"
    "operations/router.h"
    "hierarchical_component.cpp"
    "hierarchical_component.h"
    "hierarchical_system.cpp"
    "hierarchical_system.h"
    "interface.cpp"
    "operation_factory.cpp"
    "parameter.cpp"
    "parameter_creator.cpp"
    "parameter_creator.h"
    "procedural_component.cpp"
    "procedural_component_system_base.cpp"
    "procedural_object.cpp"
    "procedural_object.h"
    "procedural_object_predicate.cpp"
    "procedural_object_predicate.h"
    "procedural_object_predicate_registry.cpp"
    "procedural_object_predicate_registry.h"
    "procedural_object_system.cpp"
    "procedural_operation.cpp"
    "unknown_operation.cpp"
    "unknown_operation.h"
)

add_pagoda_module(
  NAME          objects
  SOURCES       ${OBJECTS_SRCS}
  DEPENDENCIES  common dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(GEOMETRY_UNIT_TESTS
    "procedural_object.test.cpp"
    "procedural_operation.test.cpp"
    "interface.test.cpp"
    "parameter.test.cpp"
    "parameter_creator.test.cpp"
)

foreach(unit_test_src ${GEOMETRY_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
