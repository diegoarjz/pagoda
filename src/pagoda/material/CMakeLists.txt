set(MATERIAL_SRCS
    "material.cpp"
    "material.h"
    "material_system.cpp"
    "material_system.h"
    "material_component.cpp"
    "material_component.h"
    "operations/set_material.cpp"
    "operations/set_material.h"
    "operations/set_texture.cpp"
    "operations/set_texture.h"
    "operations/set_shader.cpp"
    "operations/set_shader.cpp"
    "operations/set_material_attribute.cpp"
    "operations/set_material_attribute.h"
)

add_library(material OBJECT ${MATERIAL_SRCS})

target_include_directories(
    material
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    material
    PRIVATE
        cxx_std_17
)

target_compile_options(
    material
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    material
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    material
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

