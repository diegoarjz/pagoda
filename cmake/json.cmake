set(JSON_ROOT third_party/json CACHE STRING "Json source root")

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/${JSON_ROOT}/include)
