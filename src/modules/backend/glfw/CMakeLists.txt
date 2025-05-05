cmake_minimum_required(VERSION 3.27)
project(glfw_backend VERSION 1.0.0 LANGUAGES CXX)

add_library(glfw_backend STATIC)

target_sources(glfw_backend
    PRIVATE
        src/backend_module_factory.cpp
        src/glfw_backend.cpp
        src/glfw_backend.hpp
        src/glfw_keyboard.cpp
        src/glfw_keyboard.hpp
)

target_include_directories(glfw_backend 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/modules/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add glfw
include(third_party/glfw.cmake)

# Link libraries
target_link_libraries(glfw_backend 
    PRIVATE 
        engine::compile_options
        third_party::glfw
)

set_target_properties(glfw_backend PROPERTIES FOLDER "modules/backend")

add_library(modules::backend ALIAS glfw_backend)
