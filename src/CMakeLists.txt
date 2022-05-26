
add_library(omegalib STATIC omega_main.cc tcp_syn.cc helpers.cc pinger.cc tcp_ack.cc tcp_scanner.cc udp_scanner.cc parser.cc scan.cc threader.cc)

# ----------------------------------------------------
find_package(Threads)

target_link_libraries(omegalib ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_include_directories(omegalib PRIVATE src)

# ----------------------------------------------------

add_subdirectory(ui)

target_link_libraries(omegalib ui)
