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
        ${CMAKE_SOURCE_DIR}/engine/include
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
        entry_point
)

set_target_properties(game PROPERTIES FOLDER "game")
set_target_properties(game PROPERTIES WIN32_EXECUTABLE $<TARGET_PROPERTY:entry_point,WIN32_EXECUTABLE>)
