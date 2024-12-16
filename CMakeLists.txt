cmake_minimum_required(VERSION 3.29)
project(symphony)

set(CMAKE_CXX_STANDARD 20)

add_library(symphony SHARED
        src/search.cpp
        src/utils.cpp
        include/symphony.h
        include/problems/vacuum.h
        include/problems/simple_maze.h
        include/problems/task_scheduler.h)

target_include_directories(symphony PUBLIC include)

enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)

