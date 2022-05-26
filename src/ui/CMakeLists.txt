if(FTXUI)

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
    
    add_library(ui STATIC ftxui.cc)

    target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    target_link_libraries(omegalib
        ftxui::screen
        ftxui::dom
        ftxui::component
    )

else()

    add_library(ui STATIC ui.cc)

    target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

ENDIF()
