set(INTERPRETER_SOURCES
    "interpreter.cpp"
    "interpreter_visitor.cpp"
)

add_library(pgscript_interpreter OBJECT ${INTERPRETER_SOURCES})

target_include_directories(
    pgscript_interpreter
    PRIVATE
        $<INSTALL_INTERFACE:pagoda>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${Boost_INCLUDE_DIRS}
)

target_compile_features(
    pgscript_interpreter
    PRIVATE
        cxx_std_17
)

target_compile_options(
    pgscript_interpreter
    PRIVATE
      ${PAGODA_COMPILE_OPTIONS}
)

target_link_libraries(
    pgscript_interpreter
    PRIVATE
        common
        dynamic
)
