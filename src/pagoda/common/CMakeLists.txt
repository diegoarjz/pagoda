set(COMMON_SRCS
    "debug/assertions.cpp"
    "debug/assertions.h"
    "debug/logger.cpp"
    "debug/logger.h"
    "exception/exception.h"
    "exception/exception.cpp"
    "exception/unimplemented.cpp"
    "exception/unimplemented.h"
    "instrument/profiler.cpp"
    "instrument/profiler.h"
    "instrument/statistics.cpp"
    "instrument/statistics.h"
    "factory.h"
    "fs/file_util.cpp"
    "fs/file_util.h"
    "range.h"
    "utils.cpp"
    "utils.h"
    "version.cpp"
    "version.h"
)

add_library(common OBJECT ${COMMON_SRCS})

target_include_directories(
    common
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    common
    PRIVATE
        cxx_std_17
)

target_compile_options(
    common
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_compile_definitions(
    common
    PUBLIC
      ${PAGODA_COMPILER_ID}
    PRIVATE
        -DPAGODA_VERSION=\"${PAGODA_VERSION}\"
        -DPAGODA_BUILD_NUMBER=\"${PAGODA_BUILD_NUMBER}\"
        -DPAGODA_BUILD_DATE=\"${PAGODA_BUILD_DATE}\"
        -DPAGODA_INCLUDE_GIT_INFO=${PAGODA_INCLUDE_GIT_INFO}
        -DGIT_BRANCH=\"${GIT_BRANCH}\"
        -DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"
        -DGIT_COMMIT_AUTHOR=${GIT_COMMIT_AUTHOR}
        -DGIT_COMMIT_SUBJECT=${GIT_COMMIT_SUBJECT}
        $<$<CONFIG:DEBUG>:DEBUG>
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

