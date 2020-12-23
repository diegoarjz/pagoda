
find_package(glew REQUIRED)
find_package(Magnum REQUIRED GL Shaders)
find_package(Corrade REQUIRED Main)
find_package(OpenGL REQUIRED)

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

add_pagoda_executable("pagoda" "pagoda.cpp" "")

set(editor_libs
  "libpgeditor"
  "glfw3"
  "glew"
  "Corrade::Main"
  "Magnum::GL"
  "Magnum::GLContext"
  "Magnum::Magnum"
  "Magnum::Shaders"
  "${OPENGL_gl_LIBRARY}"
  "${COCOA_LIBRARY}"
  "${IOKIT_LIBRARY}"
  "${CORE_VIDEO_LIBRARY}"
)
add_pagoda_executable("pgeditor" "pgeditor.cpp" "${editor_libs}")
