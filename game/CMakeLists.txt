cmake_minimum_required(VERSION 3.27)
project(game VERSION 1.0.0 LANGUAGES CXX)

add_library(game STATIC)

target_sources(game
    PRIVATE
        src/create_game_instance.cpp
        src/game.cpp
        src/game.hpp
)

target_include_directories(game 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(game 
    PRIVATE 
        engine
)

set_target_properties(game PROPERTIES FOLDER "game")
