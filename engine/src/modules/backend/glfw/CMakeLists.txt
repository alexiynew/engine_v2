cmake_minimum_required(VERSION 3.27)
project(glfw_backend VERSION 1.0.0 LANGUAGES CXX)

add_library(glfw_backend STATIC)

target_sources(glfw_backend
    PRIVATE
        src/backend_module_registrar.cpp
        src/glfw_backend.cpp
        src/glfw_backend.hpp
        src/glfw_keyboard.cpp
        src/glfw_keyboard.hpp
)

target_include_directories(glfw_backend 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add glfw
include(third_party/glfw.cmake)

# Link libraries
target_link_libraries(glfw_backend 
    PRIVATE 
        engine::compile_options
        engine::modules
        third_party::glfw
)

set_target_properties(glfw_backend PROPERTIES FOLDER "engine/modules/backend")
add_library(engine::modules::backend ALIAS glfw_backend)
