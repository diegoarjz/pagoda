set(PAGODA_SOURCES
    "pagoda.h"
    "pagoda.cpp"
)

add_subdirectory(common)
add_subdirectory(geometry_core)
add_subdirectory(geometry_operations)
add_subdirectory(math_lib)
add_subdirectory(parameter)
add_subdirectory(procedural_graph)
add_subdirectory(procedural_objects)
add_subdirectory(dynamic_value)
add_subdirectory(pgscript)

add_library(libpagoda STATIC "${PAGODA_SOURCES}"
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:geometry_core>
    $<TARGET_OBJECTS:geometry_operations>
    $<TARGET_OBJECTS:math_lib>
    $<TARGET_OBJECTS:parameter>
    $<TARGET_OBJECTS:procedural_graph>
    $<TARGET_OBJECTS:procedural_graph_reader>
    $<TARGET_OBJECTS:procedural_objects>
    $<TARGET_OBJECTS:pgscript_intermediate>
    $<TARGET_OBJECTS:pgscript_interpreter>
    $<TARGET_OBJECTS:pgscript_parser>
    $<TARGET_OBJECTS:pgscript_value>
    $<TARGET_OBJECTS:dynamic_value>
)

target_include_directories(
    libpagoda
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    libpagoda
    PUBLIC
        cxx_std_17
)

target_compile_options(
    libpagoda
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    libpagoda
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)
