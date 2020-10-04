cmake_minimum_required(VERSION 3.10)
project(machine)

set(CMAKE_CXX_STANDARD 14)

add_executable(machine main.cpp interactor.h automaton.h automaton.cpp interactor.cpp)
