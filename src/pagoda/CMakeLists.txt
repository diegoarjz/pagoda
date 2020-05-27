set(PAGODA_SOURCES
    "pagoda.h"
    "pagoda.cpp"
)

add_subdirectory(common)
add_subdirectory(geometry)
add_subdirectory(image)
add_subdirectory(material)
add_subdirectory(math)
add_subdirectory(objects)
add_subdirectory(graph)
add_subdirectory(dynamic)
add_subdirectory(pgscript)

add_library(libpagoda STATIC "${PAGODA_SOURCES}"
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:geometry>
    $<TARGET_OBJECTS:image>
    $<TARGET_OBJECTS:material>
    $<TARGET_OBJECTS:math>
    $<TARGET_OBJECTS:graph>
    $<TARGET_OBJECTS:objects>
    $<TARGET_OBJECTS:pgscript>
    $<TARGET_OBJECTS:dynamic>
)

target_include_directories(
    libpagoda
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
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

target_link_libraries(
    libpagoda
    PUBLIC
        ${PNG_LIBRARY}
)

set_target_properties(libpagoda PROPERTIES PREFIX "")
set_target_properties(libpagoda PROPERTIES OUTPUT_NAME "libpagoda")
