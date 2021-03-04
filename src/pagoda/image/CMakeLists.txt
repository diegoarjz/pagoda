set(IMAGE_SRCS
    "core/image_implementation.h"
    "core/image_implementation_factory.cpp"
    "core/image_implementation_factory.h"
    "core/rgb8_image.cpp"
    "core/rgb8_image.h"
    "io/image_reader.h"
    "io/image_reader_factory.cpp"
    "io/image_reader_factory.h"
    "io/png_reader.cpp"
    "io/png_reader.h"
    "image.cpp"
    "image.h"
)

add_pagoda_module(
  NAME                image
  SOURCES             ${IMAGE_SRCS}
  DEPENDENCIES        common dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(IMAGE_UNIT_TESTS
)
