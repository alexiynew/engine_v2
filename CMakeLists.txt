cmake_minimum_required(VERSION 3.27)

project(GameEngine LANGUAGES CXX VERSION 0.0.1)

option(USE_WINMAIN "Use WinMain entry point" $<IF:$<BOOL:WIN32>,ON,OFF>)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(engine)
add_subdirectory(game)

add_subdirectory(entry_point)
