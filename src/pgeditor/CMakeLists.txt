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

add_subdirectory(common)
add_subdirectory(event)
add_subdirectory(input)
add_subdirectory(window)
add_subdirectory(gui)

set(glfw3_DIR "${CONAN_LIB_DIRS_GLFW}/cmake/glfw3")
find_package(glfw3 CONFIG REQUIRED)

add_library(libpgeditor STATIC
  $<TARGET_OBJECTS:pgeditor_common>
  $<TARGET_OBJECTS:event>
  $<TARGET_OBJECTS:input>
  $<TARGET_OBJECTS:window>
  $<TARGET_OBJECTS:gui>
)

target_include_directories(
    libpgeditor
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    libpgeditor
    PUBLIC
        cxx_std_17
)

target_compile_options(
    libpgeditor
    PRIVATE
        -Wall
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    libpgeditor
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    libpgeditor
    PRIVATE
      libpagoda
      glfw3
)

set_target_properties(libpgeditor PROPERTIES PREFIX "")
set_target_properties(libpgeditor PROPERTIES OUTPUT_NAME "libpgeditor")
