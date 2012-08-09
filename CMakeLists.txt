cmake_minimum_required(VERSION 2.6.4)
project(exact-cover C CXX)

set(CMAKE_BUILD_TYPE Debug)

include_directories(".")

file(GLOB_RECURSE test_files tests/*.cpp)

# For each file ending by tests.cpp, build an executable.
foreach (test_file ${test_files})
    # Retrieve filename without its extension.
    GET_FILENAME_COMPONENT(test_id ${test_file} NAME_WE)
    add_executable(${test_id} ${test_file})
endforeach()

