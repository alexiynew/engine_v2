cmake_minimum_required(VERSION 3.27)

project(GameProject)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(ENABLE_TESTING)
    enable_testing()
endif()

add_subdirectory(engine)
add_subdirectory(game)
