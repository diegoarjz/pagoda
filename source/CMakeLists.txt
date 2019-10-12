set(SELECTOR_PUBLIC_HEADERS
    "selector.h"
)

set(SELECTOR_SOURCES
    "selector.h"
    "selector.cpp"
)

add_subdirectory(common)
add_subdirectory(geometry_core)
add_subdirectory(geometry_operations)
add_subdirectory(math_lib)
add_subdirectory(parameter)
add_subdirectory(procedural_graph)
add_subdirectory(procedural_objects)

add_library(selector STATIC "${SELECTOR_SOURCES}"
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:geometry_core>
    $<TARGET_OBJECTS:geometry_operations>
    $<TARGET_OBJECTS:math_lib>
    $<TARGET_OBJECTS:parameter>
    $<TARGET_OBJECTS:procedural_graph>
    $<TARGET_OBJECTS:procedural_graph_reader>
    $<TARGET_OBJECTS:procedural_objects>
)

set_target_properties(
    selector
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_include_directories(
    selector
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    selector
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    selector
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    selector
    PRIVATE
        -DSELECTOR_VERSION=\"${SELECTOR_VERSION}\"
        -DSELECTOR_BUILD_NUMBER=\"${SELECTOR_BUILD_NUMBER}\"
        -DSELECTOR_BUILD_DATE=\"${SELECTOR_BUILD_DATE}\"
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    selector
    PUBLIC
        selscript
)

include(GNUInstallDirs)
install(TARGETS selector
    EXPORT selector-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/
)

install(FILES
    ${SELECTOR_COMMON_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/common/"
)

install(FILES
    ${SELECTOR_MATH_LIB_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/math_lib/"
)

install(FILES
    ${SELECTOR_GEOMCORE_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_core/"
)

install(FILES
    ${SELECTOR_GEOMETRY_OPERATION_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/geometry_operations/"
)

install(FILES
    ${SELECTOR_PARAMETER_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/parameter/"
)

install(FILES
    ${SELECTOR_PROCEDURAL_OBJECTS_PUBLIC_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_objects/"
)

install(FILES
    ${SELECTOR_PROCEDURAL_GRAPH_HEADERS}
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/selector/procedural_graph/"
)

install(EXPORT selector-export
    FILE
        SelectorTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)
