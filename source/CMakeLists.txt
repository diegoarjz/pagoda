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
add_subdirectory(dynamic_value)
add_subdirectory(selscript)

add_library(selector STATIC "${SELECTOR_SOURCES}"
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:geometry_core>
    $<TARGET_OBJECTS:geometry_operations>
    $<TARGET_OBJECTS:math_lib>
    $<TARGET_OBJECTS:parameter>
    $<TARGET_OBJECTS:procedural_graph>
    $<TARGET_OBJECTS:procedural_graph_reader>
    $<TARGET_OBJECTS:procedural_objects>
    $<TARGET_OBJECTS:selscript_intermediate>
    $<TARGET_OBJECTS:selscript_interpreter>
    $<TARGET_OBJECTS:selscript_parser>
    $<TARGET_OBJECTS:selscript_value>
    $<TARGET_OBJECTS:dynamic_value>
)

target_include_directories(
    selector
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
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
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    selector
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)
