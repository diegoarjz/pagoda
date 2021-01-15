set(EDITOR_INPUT_SRCS
    "input_manager.cpp"
    "input_manager.h"
    "mouse.h"
    "keys.h"
    "event/mouse_event.cpp"
    "event/mouse_event.h"
    "event/mouse_scroll_event.cpp"
    "event/mouse_scroll_event.h"
    "event/mouse_move_event.cpp"
    "event/mouse_move_event.h"
    "event/character_event.cpp"
    "event/character_event.h"
    "event/key_event.cpp"
    "event/key_event.h"
)

add_library(input OBJECT ${EDITOR_INPUT_SRCS})

target_include_directories(
  input
  PRIVATE
    $<INSTALL_INTERFACE:pagoda>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    ${Boost_INCLUDE_DIRS}
)

target_compile_features(
  input
  PRIVATE
    cxx_std_17
)

target_compile_options(
  input
  PRIVATE
    -Wall
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
    $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
  input
  PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
  input
  PRIVATE
    common
    "glfw3"
    "Corrade::Main"
    "Magnum::GL"
    "Magnum::GLContext"
    "Magnum::Magnum"
    "Magnum::Shaders"
    "MagnumIntegration::ImGui"
)
