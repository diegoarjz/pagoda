set(DYNAMIC_VALUE_SRCS
    "dynamic_value_base.h"
    "dynamic_value_base.cpp"
    "type_info.h"
    "type_info.cpp"
    "float_value.h"
    "float_value.cpp"
    "integer_value.h"
    "integer_value.cpp"
    "expression.h"
    "expression.cpp"
    "boolean_value.h"
    "boolean_value.cpp"
    "string_value.h"
    "string_value.cpp"
    "null_object_value.h"
    "null_object_value.cpp"
    "binding/native_value_name.h"
    "binding/native_value_name.cpp"
    "vector3.h"
    "vector3.cpp"
    "function.h"
    "function.cpp"
    "dynamic_value_table.h"
    "dynamic_value_table.cpp"
    "value_not_found.h"
    "value_not_found.cpp"
    "class_base.h"
    "class_base.cpp"
    "dynamic_class.h"
    "dynamic_class.cpp"
    "dynamic_instance.h"
    "dynamic_instance.cpp"
    "undefined_operator.h"
    "undefined_operator.cpp"
)

set(COMMON_PUBLIC_HEADERS
    "dynamic_value_base.h"
    "type_info.h"
    "float_value.h"
    "expression.h"
    "integer_value.h"
    "boolean_value.h"
    "string_value.h"
    "null_object_value.h"
    "binding/native_value_name.h"
    "vector3.h"
    "function.h"
    "dynamic_value_table.h"
    "value_not_found.h"
    "class_base.h"
    "undefined_operator.h"
    "dynamic_class.h"
    "dynamic_instance.h"
)

add_library(dynamic_value OBJECT ${DYNAMIC_VALUE_SRCS})

target_include_directories(
    dynamic_value
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
        ${SELSCRIPT_INCLUDE_DIR}
)

set_target_properties(
    dynamic_value
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    dynamic_value
    PRIVATE
        cxx_std_17
        cxx_lambdas
        cxx_constexpr
        cxx_relaxed_constexpr
)

target_compile_options(
    dynamic_value
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_compile_definitions(
    dynamic_value
    PRIVATE
        SELECTOR_PROFILER_ACTIVE
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_link_libraries(
    dynamic_value
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS dynamic_value
    EXPORT dynamic_value-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/dynamic_value
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/dynamic_value
)
install(FILES ${VALUE_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/dynamic_value)

install(EXPORT dynamic_value-export
    FILE
        BindingTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/cmake/
)


