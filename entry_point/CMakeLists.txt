
add_executable(game_entry)

target_sources(game_entry
    PRIVATE
        src/entry_point.cpp
)

target_include_directories(game_entry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/engine/include
)

target_link_libraries(game_entry
    PRIVATE
        engine
        game
)

# Setup entry point on windows
if(USE_WINMAIN)
    set_target_properties(game_entry PROPERTIES WIN32_EXECUTABLE ON)
    target_compile_definitions(game_entry PRIVATE USE_WINMAIN)
endif()
