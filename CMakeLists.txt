cmake_minimum_required(VERSION 3.26)
project(grid)

set(CMAKE_CXX_STANDARD 20)

add_executable(grid main.cpp)
target_link_libraries(grid PRIVATE -fsanitize=undefined)
target_compile_options(grid PRIVATE -fsanitize=undefined)
