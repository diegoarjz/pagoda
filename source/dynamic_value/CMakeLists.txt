set(DYNAMIC_VALUE_SRCS
    "binding/native_value_name.cpp"
    "binding/native_value_name.h"
    "boolean_value.cpp"
    "boolean_value.h"
    "builtin_class.cpp"
    "builtin_class.h"
    "callable_body_not_set.cpp"
    "callable_body_not_set.h"
    "class_base.cpp"
    "class_base.h"
    "dynamic_class.cpp"
    "dynamic_class.h"
    "dynamic_instance.cpp"
    "dynamic_instance.h"
    "dynamic_plane.cpp"
    "dynamic_plane.h"
    "dynamic_value_base.cpp"
    "dynamic_value_base.h"
    "dynamic_value_table.cpp"
    "dynamic_value_table.h"
    "expression.cpp"
    "expression.h"
    "float_value.cpp"
    "float_value.h"
    "function.cpp"
    "function.h"
    "integer_value.cpp"
    "integer_value.h"
    "null_object_value.cpp"
    "null_object_value.h"
    "register_member_function.h"
    "string_value.cpp"
    "string_value.h"
    "type_info.cpp"
    "type_info.h"
    "undefined_operator.cpp"
    "undefined_operator.h"
    "value_not_found.cpp"
    "value_not_found.h"
    "vector3.cpp"
    "vector3.h"
)

set(COMMON_PUBLIC_HEADERS
    "binding/native_value_name.h"
    "boolean_value.h"
    "builtin_class.h"
    "callable_body_not_set.h"
    "class_base.h"
    "dynamic_class.h"
    "dynamic_instance.h"
    "dynamic_plane.h"
    "dynamic_value_base.h"
    "dynamic_value_table.h"
    "expression.h"
    "float_value.h"
    "function.h"
    "integer_value.h"
    "null_object_value.h"
    "string_value.h"
    "type_info.h"
    "register_member_function.h"
    "undefined_operator.h"
    "value_not_found.h"
    "vector3.h"
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


