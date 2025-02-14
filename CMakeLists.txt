cmake_minimum_required(VERSION 3.26)

project(GameEngine LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Engine
add_executable(engine)

target_sources(engine
    PUBLIC
        include/game_engine/game.hpp
        include/game_engine/keyboard.hpp
        include/game_engine/core/engine.hpp
    PRIVATE
        src/core/engine_impl.hpp
        src/core/engine_impl.cpp
        src/game_stub.hpp
        src/game_stub.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(engine PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Entry point
add_subdirectory(src/entry_point)
target_link_libraries(engine engine::entry_point)

# Backend
add_subdirectory(src/backend)
target_link_libraries(engine engine::backend)




