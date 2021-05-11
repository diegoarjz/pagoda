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

add_pagoda_header_only(
  NAME          math
  SOURCES       ${MATH_SRCS}
  DEPENDENCIES  common
)
