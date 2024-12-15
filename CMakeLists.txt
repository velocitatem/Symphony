cmake_minimum_required(VERSION 3.29)
project(ai_rational_agents)

set(CMAKE_CXX_STANDARD 20)

add_executable(ai_rational_agents main.cpp
        lib/interface/definitions.h
        lib/interface/search.h
        lib/search.cpp)


# now for tests

add_executable(runTests tests/tests.cpp
        lib/interface/definitions.h
        lib/interface/search.h
        lib/search.cpp)

# Link the Google Test framework
target_link_libraries(runTests gtest gtest_main)
