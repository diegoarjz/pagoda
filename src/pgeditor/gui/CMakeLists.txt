list(APPEND CMAKE_MODULE_PATH "${MagnumIntegrationModules}")
find_package(Magnum REQUIRED GL Shaders)
find_package(Corrade REQUIRED Main)
find_package(OpenGL REQUIRED)
find_package(MagnumIntegration REQUIRED ImGui)

if(CORRADE_TARGET_APPLE)
  find_package(Magnum REQUIRED CglContext)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(CORE_VIDEO_LIBRARY CoreVideo)
elseif(CORRADE_TARGET_UNIX)
  find_package(Magnum REQUIRED GlxContext)
elseif(CORRADE_TARGET_WINDOWS)
  find_package(Magnum REQUIRED WglContext)
else()
  message(FATAL_ERROR "No context handler available on this platform")
endif()


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
