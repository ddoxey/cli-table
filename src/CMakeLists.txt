cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wfatal-errors)

project(cli-table)

include_directories(
    inc
    /usr/local/boost_1_62_0
)
link_directories(/usr/local/boost_1_62_0/stage/lib)

add_library(tablib inc/table.cpp inc/table/cell.cpp)

add_executable(table main.cpp inc/table.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(table
    tablib 
    boost_filesystem
    boost_system
    boost_program_options
)
