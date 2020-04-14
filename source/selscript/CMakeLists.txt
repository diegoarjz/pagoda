add_subdirectory(intermediate)
add_subdirectory(value)
add_subdirectory(parser)
add_subdirectory(interpreter)

add_library(selscript OBJECT
    $<TARGET_OBJECTS:selscript_intermediate>
    $<TARGET_OBJECTS:selscript_interpreter>
    $<TARGET_OBJECTS:selscript_parser>
    $<TARGET_OBJECTS:selscript_value>
)

target_include_directories(
    selscript
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    selscript
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript
    PRIVATE
        -Wall
        -Werror
)

target_link_libraries(
    selscript
    PRIVATE
        common
)

set_target_properties(
    selscript
    PROPERTIES
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

include(GNUInstallDirs)
install(TARGETS selscript
    EXPORT selscript-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/
)

install(EXPORT selscript-export
    FILE
        SelScriptTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
