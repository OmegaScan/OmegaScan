add_library(omegalib STATIC omega_main.cc tcp_syn.cc helpers.cc pinger.cc)
target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
