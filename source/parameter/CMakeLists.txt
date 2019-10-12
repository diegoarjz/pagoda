set(PARAMETER_SRCS
    "context.cpp"
    "context.h"
    "expression.cpp"
    "expression.h"
    "parameter.cpp"
    "parameter.h"
    "parameter_exception.cpp"
    "parameter_exception.h"
    "parameterizable.h"
    "variable.cpp"
    "variable.h"
    "variable_resolver.cpp"
    "variable_resolver.h"
)

set(PARAMETER_PUBLIC_HEADERS
    "context.h"
    "expression.h"
    "parameter.h"
    "parameter_exception.h"
    "parameterizable.h"
    "variable.h"
    "variable_resolver.h"
)

add_library(parameter OBJECT ${PARAMETER_SRCS})

target_include_directories(
    parameter
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    parameter
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    parameter
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    parameter
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    parameter
    PRIVATE
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    parameter
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS parameter
    EXPORT parameter-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/parameter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/parameter
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/parameter)

install(EXPORT parameter-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


