set(profiling_srcs
    profile_vec_base.cpp
    profile_vec_arithmetics.cpp
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

endforeach(source ${profiling_srcs})
