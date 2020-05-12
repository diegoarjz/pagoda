set(GEOMETRY_OPERATIONS_SRCS
    "create_box.h"
    "create_rect.h"
    "create_sphere.h"
    "ear_clipping.h"
    "explode_to_faces.h"
    "extrusion.h"
    "extrusion_profile.h"
    "face_offset.h"
    "plane_splits.h"
    "triangulate.h"
)

add_library(geometry_operations OBJECT ${GEOMETRY_OPERATIONS_SRCS})

target_include_directories(
    geometry_operations
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    geometry_operations
    PRIVATE
        cxx_std_17
)

target_compile_options(
    geometry_operations
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    geometry_operations
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    geometry_operations
    PRIVATE
        common
)
