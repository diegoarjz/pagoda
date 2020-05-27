set(RENDERING_SRCS
    "bound_state.h"
    "geometry.cpp"
    "geometry.h"
    "gl_debug.cpp"
    "gl_debug.h"
    "material.cpp"
    "material.h"
    "renderable.cpp"
    "renderable.h"
    "renderable_object.cpp"
    "renderable_object.h"
    "renderer.cpp"
    "renderer.h"
    "shader.cpp"
    "shader.h"
    "shader_program.cpp"
    "shader_program.h"
    "texture.cpp"
    "texture.h"
    "uniform.cpp"
    "uniform.h"
    "vertex.h"
)

add_library(rendering OBJECT ${RENDERING_SRCS})

target_include_directories(
    rendering
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    rendering
    PRIVATE
        cxx_std_17
)

target_compile_options(
    rendering
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    rendering
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    rendering
    PRIVATE
        common
)
