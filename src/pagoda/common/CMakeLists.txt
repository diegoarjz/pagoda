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
    "fs/file_util.cpp"
    "fs/file_util.h"
    "range.h"
    "utils.cpp"
    "utils.h"
    "plugin.cpp"
    "version.cpp"
)

set(COMMON_COMPILE_DEFINITIONS
  -DPAGODA_VERSION=\"${PAGODA_VERSION}\"
  -DPAGODA_BUILD_NUMBER=\"${PAGODA_BUILD_NUMBER}\"
  -DPAGODA_BUILD_DATE=\"${PAGODA_BUILD_DATE}\"
  -DPAGODA_INCLUDE_GIT_INFO=${PAGODA_INCLUDE_GIT_INFO}
  -DGIT_BRANCH=\"${GIT_BRANCH}\"
  -DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"
  -DGIT_COMMIT_AUTHOR=${GIT_COMMIT_AUTHOR}
  -DGIT_COMMIT_SUBJECT=${GIT_COMMIT_SUBJECT}
)
if (${PAGODA_ENABLE_ASSERTIONS})
  list(APPEND COMMON_COMPILE_DEFINITIONS "-DPAGODA_ENABLE_ASSERTIONS=\"${PAGODA_ENABLE_ASSERTIONS}\"")
endif()
if (${PAGODA_PROFILER_ACTIVE})
  list(APPEND COMMON_COMPILE_DEFINITIONS "-DPAGODA_PROFILER_ACTIVE=\"${PAGODA_PROFILER_ACTIVE}\"")
endif()

add_pagoda_module(
  NAME                common
  SOURCES             ${COMMON_SRCS}
  COMPILE_DEFINITIONS "${COMMON_COMPILE_DEFINITIONS}"
  DEPENDENCIES        nlohmann_json::nlohmann_json
)


#########################################################################
# Unit tests
#########################################################################

set(COMMON_UNIT_TESTS
    "utils.test.cpp"
)

foreach(unit_test_src ${COMMON_UNIT_TESTS})
    add_unit_test(${unit_test_src})
endforeach()
