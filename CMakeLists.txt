cmake_minimum_required(VERSION 3.9)
project(omegascan)

add_subdirectory(src)

add_executable(omegascan main.cc)
target_link_libraries(omegascan omegalib)
