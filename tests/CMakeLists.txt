cmake_minimum_required(VERSION 3.0)
project(automatons)
set(CMAKE_CXX_STANDARD 17)
include_directories(..)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
message(${CMAKE_GENERATOR})
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} —build .
RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
${CMAKE_CURRENT_BINARY_DIR}/googletest-build
EXCLUDE_FROM_ALL)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES ../automaton.cpp)

file(GLOB_RECURSE TEST_SOURCES test_automaton.cpp)
add_executable(tester ${TEST_SOURCES} ${SOURCES})
target_include_directories(tester PRIVATE "${gtest_SOURCE_DIR}/include")
target_link_libraries(tester gtest_main)