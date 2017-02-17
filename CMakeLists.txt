cmake_minimum_required(VERSION 3.6)
project(TimSort)

find_package(Threads)
find_package(GTest)

add_compile_options(-O2)
link_libraries(pthread)
link_libraries(gtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        test.cpp
        timsort_params.h Runs.h TimSort.h InplaceMerge.h
        )
add_executable(TimSort ${SOURCE_FILES})

target_link_libraries(TimSort pthread gtest)