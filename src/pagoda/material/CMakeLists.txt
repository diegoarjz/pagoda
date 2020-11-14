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
      ${PAGODA_COMPILE_OPTIONS}
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
        dynamic
)

#########################################################################
# Unit tests
#########################################################################

set(IMAGE_UNIT_TESTS
)
