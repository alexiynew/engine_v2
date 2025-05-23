cmake_minimum_required(VERSION 3.27)
project(stub_graphics VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_graphics STATIC)

target_sources(stub_graphics
    PRIVATE
        src/graphics_create.cpp
        src/stub_mesh.cpp
        src/stub_mesh.hpp
        src/stub_shader.cpp
        src/stub_shader.hpp
        src/stub_renderer.cpp
        src/stub_renderer.hpp
)

target_include_directories(stub_graphics 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(stub_graphics 
    PRIVATE 
        engine::compile_options
        engine::modules
)

set_target_properties(stub_graphics PROPERTIES FOLDER "engine/modules/graphics")
add_library(engine::modules::graphics ALIAS stub_graphics)
