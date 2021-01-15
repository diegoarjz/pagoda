set(EVENT_SRCS
  "event_manager.cpp"
  "event_manager.h"
)

add_library(event OBJECT ${EVENT_SRCS})

target_include_directories(
    event
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    event
    PRIVATE
        cxx_std_17
)

target_compile_options(
    event
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    event
    PRIVATE
        $<$<CONFIG:DEBUG>:DEBUG>
)

target_link_libraries(
    event
    PRIVATE
        common
)

#########################################################################
# Unit tests
#########################################################################
set(EVENT_UNIT_TESTS
  "event_manager.test.cpp"
)

foreach(unit_test_src ${EVENT_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
