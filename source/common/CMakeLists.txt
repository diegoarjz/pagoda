set(COMMON_SRCS
    "assertions.cpp"
    "assertions.h"
    "exception.h"
    "exception.cpp"
    "factory.h"
    "file_util.cpp"
    "file_util.h"
    "logger.cpp"
    "logger.h"
    "profiler.cpp"
    "profiler.h"
    "range.h"
    "statistics.cpp"
    "statistics.h"
    "unimplemented.cpp"
    "unimplemented.h"
    "utils.cpp"
    "utils.h"
    "version.cpp"
    "version.h"
)

set(COMMON_PUBLIC_HEADERS
    "assertions.h"
    "exception.h"
    "const_str.h"
    "factory.h"
    "file_util.h"
    "logger.h"
    "profiler.h"
    "range.h"
    "statistics.h"
    "unimplemented.h"
    "utils.h"
    "version.h"
)

add_library(common OBJECT ${COMMON_SRCS})

target_include_directories(
    common
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${PGSCRIPT_INCLUDE_DIR}
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
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_compile_definitions(
    common
    PRIVATE
        -DPAGODA_VERSION=\"${PAGODA_VERSION}\"
        -DPAGODA_BUILD_NUMBER=\"${PAGODA_BUILD_NUMBER}\"
        -DPAGODA_BUILD_DATE=\"${PAGODA_BUILD_DATE}\"
        -DPAGODA_INCLUDE_GIT_INFO=${PAGODA_INCLUDE_GIT_INFO}
        -DGIT_BRANCH=\"${GIT_BRANCH}\"
        -DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"
        -DGIT_COMMIT_AUTHOR=${GIT_COMMIT_AUTHOR}
        -DGIT_COMMIT_SUBJECT=${GIT_COMMIT_SUBJECT}
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

if (${PAGODA_ENABLE_ASSERTIONS})
    target_compile_options(common PUBLIC -DPAGODA_ENABLE_ASSERTIONS="${PAGODA_ENABLE_ASSERTIONS}")
endif()
if (${PAGODA_PROFILER_ACTIVE})
    target_compile_options(common PUBLIC -DPAGODA_PROFILER_ACTIVE="${PAGODA_PROFILER_ACTIVE}")
endif()

target_link_libraries(
    common
    PRIVATE
        Boost::filesystem
)

