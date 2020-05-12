set(MATH_SRCS
    "bissectrix.h"
    "degrees.h"
    "intersection.h"
    "math_utils.h"
    "line_3d.h"
    "line_segment_3d.h"
    "matrix_base.h"
    "nearest_points.h"
    "orthogonal.h"
    "plane.h"
    "projection.h"
    "radians.h"
    "vec_base.h"
)

add_library(math OBJECT ${MATH_SRCS})

target_include_directories(
    math
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    math
    PRIVATE
        cxx_std_17
)

target_compile_options(
    math
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    math
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    math
    PRIVATE
        common
)
