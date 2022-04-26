add_library(omegalib STATIC omega_main.cc tcp_syn.cc helpers.cc pinger.cc tcp_ack.cc tcp_scanner.cc udp_scanner.cc parser.cc scan.cc ui.cc)

set(ftxui_DIR ${VCPKG_ROOT}/installed/x64-linux/share/ftxui)
add_library(fftxui STATIC ftxui.cc)
find_package(ftxui CONFIG REQUIRED)
target_link_libraries(fftxui PRIVATE ftxui::dom ftxui::screen ftxui::component)

target_link_libraries(omegalib PRIVATE fftxui)

target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(fftxui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
