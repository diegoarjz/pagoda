set(WGS84toCartesian_ROOT third_party/WGS84toCartesian CACHE STRING "WGS84toCartesian source root")

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/${WGS84toCartesian_ROOT})
