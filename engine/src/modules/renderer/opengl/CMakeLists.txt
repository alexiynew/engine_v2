cmake_minimum_required(VERSION 3.27)
project(opengl_renderer VERSION 1.0.0 LANGUAGES CXX)

add_library(opengl_renderer STATIC)

target_sources(opengl_renderer
    PRIVATE
        src/renderer_module_factory.cpp
        src/opengl_mesh.cpp
        src/opengl_mesh.hpp
        src/opengl_shader.cpp
        src/opengl_shader.hpp
        src/opengl_utils.cpp
        src/opengl_utils.hpp
        src/opengl_renderer.cpp
        src/opengl_renderer.hpp
)

target_include_directories(opengl_renderer 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add glad
add_subdirectory(third_party/glad)
include(third_party/glm)

# Link libraries
target_link_libraries(opengl_renderer 
    PRIVATE 
        engine::compile_options
        engine::modules
        third_party::glad
        third_party::glm
)

set_target_properties(opengl_renderer PROPERTIES FOLDER "engine/modules/renderer")

add_library(modules::renderer ALIAS opengl_renderer)
