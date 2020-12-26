add_subdirectory(input)
add_subdirectory(window)

set(glfw3_DIR "${CONAN_LIB_DIRS_GLFW}/cmake/glfw3")
find_package(glfw3 CONFIG REQUIRED)

add_library(libpgeditor STATIC
  $<TARGET_OBJECTS:input>
  $<TARGET_OBJECTS:window>
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
