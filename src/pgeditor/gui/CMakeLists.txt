set(GUI_SRCS
  "gui_manager.cpp"
  "gui_manager.h"
)

add_library(gui OBJECT ${GUI_SRCS})

target_include_directories(
    gui
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    gui
    PRIVATE
        cxx_std_17
)

target_compile_options(
    gui
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    gui
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
  gui
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
