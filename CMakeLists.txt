cmake_minimum_required(VERSION 3.9)
project(omegascan)


set(CMAKE_CXX_COMPILER "/usr/bin/g++")

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)

add_subdirectory(src)
add_subdirectory(test)

add_executable(omegascan main.cc)
target_link_libraries(omegascan omegalib)
