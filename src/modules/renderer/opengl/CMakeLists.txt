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
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/modules/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add glad
add_subdirectory(third_party/glad)
include(libs/glm)

# Link libraries
target_link_libraries(opengl_renderer 
    PRIVATE 
        engine::compile_options
        third_party::glad
        libs::glm
)

add_library(engine::renderer ALIAS opengl_renderer)
