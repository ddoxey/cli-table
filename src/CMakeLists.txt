cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -pedantic -Werror)

project(cli-table)

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")

include_directories(
    /usr/src/googletest/googletest/include
    /usr/include
    inc
)

find_package(Boost 1.67 REQUIRED COMPONENTS thread)

add_library(tablib table.cpp inc/table/cell.cpp)

add_executable(table main.cpp inc/table.hpp)

target_link_libraries(table
    tablib 
)

add_executable(test_table test/test_table.cpp)

target_link_libraries(test_table
    tablib 
    gtest
    Boost::thread
)
