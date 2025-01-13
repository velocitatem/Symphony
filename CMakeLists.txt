cmake_minimum_required(VERSION 3.29)
project(symphony)

set(CMAKE_CXX_STANDARD 20)

add_library(symphony SHARED
        src/search.cpp
        src/utils.cpp
        include/symphony.h
        include/problems/vacuum.h
        include/problems/simple_maze.h
        include/problems/task_scheduler.h
        include/problems/study_path.h)

target_include_directories(symphony PUBLIC include)

# add openmp
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(symphony PUBLIC OpenMP::OpenMP_CXX)
endif()

enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)

