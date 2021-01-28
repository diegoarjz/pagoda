set(WINDOW_SRCS
    "window.cpp"
    "window.h"
    "window_creation_params.cpp"
    "window_creation_params.h"
    "event/quit_event.cpp"
    "event/quit_event.h"
    "event/window_resize_event.cpp"
    "event/window_resize_event.h"
)

add_library(window OBJECT ${WINDOW_SRCS})

target_include_directories(
    window
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    window
    PRIVATE
        cxx_std_17
)

target_compile_options(
    window
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    window
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    window
    PRIVATE
        common
)
