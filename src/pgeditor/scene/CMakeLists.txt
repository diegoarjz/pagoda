set(SCENE_SRCS
    "attachable.h"
    "attachable_visitor.h"
    "camera.cpp"
    "camera.h"
    "camera_manager.cpp"
    "camera_manager.h"
    "camera_rig.cpp"
    "camera_rig.h"
    "geometry.cpp"
    "geometry.h"
    "lens.cpp"
    "lens.h"
    "mesh.cpp"
    "mesh.h"
    "node.cpp"
    "node.h"
    "transformation.cpp"
    "transformation.h"
)

add_library(scene OBJECT ${SCENE_SRCS})

target_include_directories(
    scene
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    scene
    PRIVATE
        cxx_std_17
)

target_compile_options(
    scene
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    scene
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    scene
    PRIVATE
        common
)
