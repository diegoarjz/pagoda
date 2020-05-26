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
)

target_compile_features(
    image
    PRIVATE
        cxx_std_17
)

target_compile_options(
    image
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
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
)

#########################################################################
# Unit tests
#########################################################################

set(IMAGE_UNIT_TESTS
)

foreach(unit_test_src ${IMAGE_UNIT_TESTS})
    get_filename_component(unit_test_base_name ${unit_test_src} NAME)
    string(REPLACE ".test.cpp" "_test" test_name ${unit_test_base_name})
    message(STATUS "src: ${unit_test_src} name: ${test_name}")

    add_executable(${test_name} ${unit_test_src})

    target_include_directories(
        ${test_name}
        PRIVATE
            $<INSTALL_INTERFACE:pagoda>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
            ${Boost_INCLUDE_DIRS}
    )

    target_compile_features(
        ${test_name}
        PRIVATE
            cxx_std_17
    )

    target_compile_options(
        ${test_name}
        PRIVATE
            -Wall
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
            $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
    )

    target_compile_definitions(
        ${test_name}
        PRIVATE
            $<$<CONFIG:DEBUG>:DEBUG>
    )

    target_link_libraries(
        ${test_name}
        PRIVATE
            libpagoda
            gmock
            gtest
            pthread
    )

add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
