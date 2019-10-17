set(INTERPRETER_SOURCES
    "interpreter.cpp"
    "interpreter_visitor.cpp"
    "symbol_table.cpp"
)

set(INTERPRETER_PUBLIC_HEADERS
    "interpreter.h"
    "interpreter_visitor.h"
    "symbol_table.h"
)

add_library(interpreter OBJECT ${INTERPRETER_SOURCES})

target_include_directories(
    interpreter
    PRIVATE
        $<INSTALL_INTERFACE:selector>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    interpreter
    PRIVATE
        cxx_std_17
)

target_compile_options(
    interpreter
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    interpreter
    PRIVATE
)

include(GNUInstallDirs)
install(TARGETS interpreter
    EXPORT interpreter-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/interpreter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/selector/selscript/interpreter
)
install(FILES ${INTERPRETER_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/selector/selscript/interpreter)

install(EXPORT interpreter-export
    FILE
        InterpreterTargets.cmake
    NAMESPACE
        Selector::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/selector/selscript/cmake/
)
