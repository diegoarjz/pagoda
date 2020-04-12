set(MATH_LIB_SRCS
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
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

target_compile_features(
    math_lib
    PRIVATE
        cxx_std_17
)

target_compile_options(
    math_lib
    PRIVATE
        -Wall
        -Werror
)

target_link_libraries(
    math_lib
    PUBLIC
        common
)

include(GNUInstallDirs)
install(TARGETS math_lib
    EXPORT math_lib-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/math_lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/math_lib
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/math_lib)

install(EXPORT math_lib-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


