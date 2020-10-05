cmake_minimum_required(VERSION 3.10)
project(Google_tests)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(G_test)
include_directories(..)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run main.cpp test_automaton.cpp ../automaton.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)