# Check renderer type
set(SUPPORTED_RENDERERS opengl stub)

if(NOT ENGINE_RENDERER_TYPE IN_LIST SUPPORTED_RENDERERS)
    message(FATAL_ERROR "Renderer ${ENGINE_RENDERER_TYPE} isn't supported.")
endif()

# Renderer library
add_library(renderer STATIC)

target_sources(renderer
    PUBLIC
        renderer.hpp
)

target_include_directories(renderer 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(renderer PROPERTIES FOLDER "renderer")

include(${ENGINE_RENDERER_TYPE}/CMakeLists.txt)
include(libs/glm)

target_link_libraries(renderer 
    PRIVATE 
        engine::compile_options
        libs::glm
)

add_library(engine::renderer ALIAS renderer)
