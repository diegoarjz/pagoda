set(BGFX_ROOT third_party/bgfx CACHE STRING "BGFX source root")
set(BX_ROOT third_party/bx CACHE STRING "BX source root")
set(BIMG_ROOT third_party/bimg CACHE STRING "BIMG source root")

set(BGFX_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/${BGFX_ROOT}/include
  ${PROJECT_SOURCE_DIR}/${BX_ROOT}/include
  ${PROJECT_SOURCE_DIR}/${BIMG_ROOT}/include
  )

if (${APPLE})
  set(BGFX_MAKE_TARGET "osx-release62")
  set(BGFX_LIBRARY
    ${PROJECT_SOURCE_DIR}/${BGFX_ROOT}/.build/osx64_clang/bin/libbgfxRelease.a)
elseif(${UNIX})
  set(BGFX_MAKE_TARGET "linux-release64")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(BGFX_LIBRARY
      ${PROJECT_SOURCE_DIR}/${BGFX_ROOT}/.build/linux64_clang/bin/libbgfxRelease.a)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(BGFX_LIBRARY
      ${PROJECT_SOURCE_DIR}/${BGFX_ROOT}/.build/linux64_gcc/bin/libbgfxRelease.a)
  endif()
endif()

execute_process(
  COMMAND make -j4 ${BGFX_MAKE_TARGET}
  WORKING_DIRECTORY ${BGFX_ROOT}
  )

if (NOT EXISTS ${BGFX_LIBRARY})
  message(FATAL "BGFX Library does not exist at ${BGFX_LIBRARY}")
endif()
