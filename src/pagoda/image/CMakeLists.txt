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

add_library(image OBJECT ${IMAGE_SRCS})

target_include_directories(
    image
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
    PUBLIC
        ${PNG_INCLUDE_DIRS}
)

target_compile_features(
    image
    PRIVATE
        cxx_std_17
)

target_compile_options(
    image
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    image
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    image
    PRIVATE
        common
        dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(IMAGE_UNIT_TESTS
)
