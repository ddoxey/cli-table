cmake_minimum_required(VERSION 3.22.2)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -pedantic -Werror)

project(cli-table)

set(CMAKE_INSTALL_PREFIX "/usr/local/")
set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")

include_directories(
    /usr/local/src/googletest/googletest/include
    /usr/src/googletest/googletest/include
    /usr/local/include
    /usr/include
    src/
)

add_library(tablib src/table.cpp)

add_executable(table src/main.cpp src/table.hpp)

target_link_libraries(table
    tablib 
)

add_executable(test_table src/test/test_table.cpp)

target_link_libraries(test_table
    tablib 
    gtest
    pthread
)

install(
    TARGETS table RUNTIME
    DESTINATION bin/
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

install(
    TARGETS tablib LIBRARY 
    DESTINATION lib/
    PERMISSIONS OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
)

install(FILES src/table.hpp
        DESTINATION include/cli-table/
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ
)
