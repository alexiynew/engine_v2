cmake_minimum_required(VERSION 3.27)
project(stub_renderer VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_renderer STATIC)

target_sources(stub_renderer
    PRIVATE
        src/renderer_module_factory.cpp
        src/stub_mesh.cpp
        src/stub_mesh.hpp
        src/stub_shader.cpp
        src/stub_shader.hpp
        src/stub_renderer.cpp
        src/stub_renderer.hpp
)

target_include_directories(stub_renderer 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(stub_renderer 
    PRIVATE 
        engine::compile_options
        engine::modules
)

set_target_properties(stub_renderer PROPERTIES FOLDER "engine/modules/renderer")
add_library(engine::modules::renderer ALIAS stub_renderer)
