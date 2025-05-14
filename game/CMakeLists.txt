cmake_minimum_required(VERSION 3.27)
project(game VERSION 1.0.0 LANGUAGES CXX)

add_executable(game)

target_sources(game
    PRIVATE
        src/create_game_instance.cpp
        src/game.cpp
        src/game.hpp
)

target_include_directories(game
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(game 
    PRIVATE 
        cxx_std_23
)

# Link libraries
target_link_libraries(game
    PRIVATE 
        engine
        engine::entry_point
)

set_target_properties(game PROPERTIES
    FOLDER "game"
    EXPORT_COMPILE_COMMANDS ON
    # It is necessary to get the executable type from the engine in order to set the main\WinMain entry point correctly.
    WIN32_EXECUTABLE $<TARGET_PROPERTY:engine,WIN32_EXECUTABLE>
)
