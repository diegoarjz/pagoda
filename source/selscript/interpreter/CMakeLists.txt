set(INTERPRETER_SOURCES
    "interpreter.cpp"
    "interpreter_visitor.cpp"
)

set(INTERPRETER_PUBLIC_HEADERS
    "interpreter.h"
    "interpreter_visitor.h"
)

add_library(selscript_interpreter OBJECT ${INTERPRETER_SOURCES})

target_include_directories(
    selscript_interpreter
    PUBLIC
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    selscript_interpreter
    PUBLIC
        common
)

target_compile_features(
    selscript_interpreter
    PRIVATE
        cxx_std_17
)

target_compile_options(
    selscript_interpreter
    PRIVATE
        -Wall
        -Werror
)

include(GNUInstallDirs)
install(TARGETS selscript_interpreter
    EXPORT selscript_interpreter-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_interpreter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/selscript_interpreter
)
install(FILES ${INTERPRETER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/selscript_interpreter)

install(EXPORT selscript_interpreter-export
    FILE
        InterpreterTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
