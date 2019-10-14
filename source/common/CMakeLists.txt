set(COMMON_SRCS
    "assertions.cpp"
    "assertions.h"
    "const_str.h"
    "factory.h"
    "file_util.cpp"
    "file_util.h"
    "logger.cpp"
    "logger.h"
    "profiler.cpp"
    "profiler.h"
    "pugiconfig.hpp"
    "pugixml.cpp"
    "pugixml.hpp"
    "range.h"
    "statistics.cpp"
    "statistics.h"
    "utils.cpp"
    "utils.h"
    "version.cpp"
    "version.h"
)

set(COMMON_PUBLIC_HEADERS
    "assertions.h"
    "const_str.h"
    "factory.h"
    "file_util.h"
    "logger.h"
    "profiler.h"
    "pugiconfig.hpp"
    "pugixml.hpp"
    "range.h"
    "statistics.h"
    "utils.h"
    "version.h"
)

add_library(common OBJECT ${COMMON_SRCS})

target_include_directories(
    common
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    common
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    common
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    common
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    common
    PRIVATE
        -DSELECTOR_VERSION=\"${SELECTOR_VERSION}\"
        -DSELECTOR_BUILD_NUMBER=\"${SELECTOR_BUILD_NUMBER}\"
        -DSELECTOR_BUILD_DATE=\"${SELECTOR_BUILD_DATE}\"
        -DASSERTS_ENABLED=${Selector_ENABLE_ASSERTIONS}
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    common
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS common
    EXPORT common-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/common
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/common
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/common)

install(EXPORT common-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)
