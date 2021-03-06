set(MATH_LIB_SRCS
    "bissectrix.h"
    "degrees.h"
    "intersection.h"
    "length.h"
    "math_utils.h"
    "line_3d.h"
    "line_segment_3d.h"
    "matrix_base.h"
    "nearest_points.h"
    "orthogonal.h"
    "plane.h"
    "projection.h"
    "radians.h"
    "transpose.h"
    "vec_base.h"
)

set(MATH_LIB_PUBLIC_HEADERS
    "bissectrix.h"
    "degrees.h"
    "intersection.h"
    "length.h"
    "line_3d.h"
    "line_segment_3d.h"
    "math_utils.h"
    "matrix_base.h"
    "nearest_points.h"
    "orthogonal.h"
    "plane.h"
    "projection.h"
    "radians.h"
    "transpose.h"
    "vec_base.h"
)

add_library(math_lib OBJECT ${MATH_LIB_SRCS})

target_include_directories(
    math_lib
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    math_lib
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    math_lib
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    math_lib
    PRIVATE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    math_lib
    PRIVATE
        common
)
