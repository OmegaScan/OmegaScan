add_library(omegalib STATIC omega_main.cc tcp_syn.cc helpers.cc pinger.cc tcp_scanner.cc udp_scanner.cc parser.cc scan.cc ui.cc)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Important: Specify a GIT_TAG XXXXX here.
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

target_include_directories(omegalib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(omegalib PRIVATE src)

target_link_libraries(omegalib
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
