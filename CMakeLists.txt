cmake_minimum_required(VERSION 3.29)
project(ai_rational_agents)

set(CMAKE_CXX_STANDARD 20)

add_library(ai_rational_agents SHARED
        src/search.cpp
        src/utils.cpp)

target_include_directories(ai_rational_agents PUBLIC include)

enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)

