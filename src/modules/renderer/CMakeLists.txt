# Check renderer type
set(SUPPORTED_RENDERERS opengl stub)

if(NOT ENGINE_RENDERER_TYPE IN_LIST SUPPORTED_RENDERERS)
    message(FATAL_ERROR "Renderer ${ENGINE_RENDERER_TYPE} isn't supported.")
endif()

# Renderer library
add_library(renderer STATIC)

target_sources(renderer
    PUBLIC
        include/renderer/renderer.hpp
)

target_include_directories(renderer 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(renderer PROPERTIES FOLDER "renderer")

include(src/${ENGINE_RENDERER_TYPE}/CMakeLists.txt)
include(libs/glm)

target_link_libraries(renderer 
    PRIVATE 
        engine::compile_options
        libs::glm
)
