set(VALUE_SOURCES
    "builtin_functions.cpp"
    "builtin_functions.h"
    "script_callable_body.h"
    "script_callable_body.cpp"
)

add_library(pgscript_value OBJECT ${VALUE_SOURCES})

target_include_directories(
    pgscript_value
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgscript_value
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgscript_value
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-gnu-zero-variadic-macro-arguments>
)

target_link_libraries(
    pgscript_value
    PRIVATE
        common
        dynamic
)
