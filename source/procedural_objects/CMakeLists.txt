set(PROCEDURAL_OBJECTS_SRCS
    "clip_geometry.cpp"
    "clip_geometry.h"
    "create_box.cpp"
    "create_box.h"
    "create_rect.cpp"
    "create_rect.h"
    "export_geometry.cpp"
    "export_geometry.h"
    "extract_faces.cpp"
    "extract_faces.h"
    "extract_scope.cpp"
    "extract_scope.h"
    "extrude_geometry.cpp"
    "extrude_geometry.h"
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
    "repeat_split.cpp"
    "repeat_split.h"
    "rotate.cpp"
    "rotate.h"
    "scale.cpp"
    "scale.h"
    "scope_axis_direction_predicate.cpp"
    "scope_axis_direction_predicate.h"
    "split.cpp"
    "split.h"
    "translate.cpp"
    "translate.h"
    "triangulate_geometry.cpp"
    "triangulate_geometry.h"
    "unknown_operation.cpp"
    "unknown_operation.h"
)

set(PROCEDURAL_OBJECTS_PUBLIC_HEADERS
    "clip_geometry.h"
    "create_box.h"
    "create_rect.h"
    "export_geometry.h"
    "extract_faces.h"
    "extract_scope.h"
    "extrude_geometry.h"
    "geometry_component.h"
    "geometry_system.h"
    "hierarchical_component.h"
    "hierarchical_system.h"
    "operation_factory.h"
    "procedural_component.h"
    "procedural_component_system.h"
    "procedural_component_system_base.h"
    "procedural_object.h"
    "procedural_object_mask.h"
    "procedural_object_predicate.h"
    "procedural_object_predicate_registry.h"
    "procedural_object_system.h"
    "procedural_operation.h"
    "procedural_operation_object_interface.h"
    "repeat_split.h"
    "rotate.h"
    "scale.h"
    "scope_axis_direction_predicate.h"
    "split.h"
    "translate.h"
    "triangulate_geometry.h"
    "unknown_operation.h"
)

add_library(procedural_objects OBJECT ${PROCEDURAL_OBJECTS_SRCS})

target_include_directories(
    procedural_objects
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    procedural_objects
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    procedural_objects
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    procedural_objects
    PRIVATE
        -Wall
        -Werror
)

target_compile_definitions(
    procedural_objects
    PRIVATE
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    procedural_objects
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS procedural_objects
    EXPORT procedural_objects-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/procedural_objects
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/procedural_objects
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_objects)

install(EXPORT procedural_objects-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


