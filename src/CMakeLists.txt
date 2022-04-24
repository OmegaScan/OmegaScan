add_library(omegalib STATIC omega_main.cc tcp_syn.cc helpers.cc pinger.cc tcp_scanner.cc udp_scanner.cc parser.cc scan.cc ui.cc)
target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
