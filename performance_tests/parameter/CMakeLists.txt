set(profiling_srcs
    parameter_resolver.cpp
    )

foreach(source ${profiling_srcs})
    string(REPLACE ".cpp" "" exec_name ${source})
    add_executable(
        ${exec_name}
        ${source}
        "../main.cpp"
    )

    target_include_directories(
        ${exec_name}
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
    )

    target_link_libraries(
        ${exec_name}
        PRIVATE
            selector
            Boost::chrono
            Boost::system
    )

    target_compile_features(
        ${exec_name}
        PRIVATE
            cxx_std_14
            cxx_lambdas
    )

    target_compile_definitions(
        ${exec_name}
        PRIVATE
            -DSELECTOR_VERSION=\"${SELECTOR_VERSION}\"
            -DSELECTOR_BUILD_NUMBER=\"${SELECTOR_BUILD_NUMBER}\"
            -DSELECTOR_BUILD_DATE=\"${SELECTOR_BUILD_DATE}\"
            -DSEL_MODIFIER=
    )

endforeach(source ${profiling_srcs})
