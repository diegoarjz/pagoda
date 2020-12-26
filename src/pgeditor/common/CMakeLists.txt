set(PGEDITOR_COMMON_SRCS
  "manager.cpp"
  "manager.h"
)

add_library(pgeditor_common OBJECT ${PGEDITOR_COMMON_SRCS})

target_include_directories(
    pgeditor_common
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgeditor_common
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgeditor_common
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    pgeditor_common
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    pgeditor_common
    PRIVATE
        common
)
