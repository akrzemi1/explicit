project(explicit)
cmake_minimum_required(VERSION 2.6)
enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra ")

include_directories(include)
add_executable(test_explicit test/test_explicit.cpp)

add_test(test_explicit test_explicit)
