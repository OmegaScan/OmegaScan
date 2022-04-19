add_library(omegalib STATIC omega_main.cc)
target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
