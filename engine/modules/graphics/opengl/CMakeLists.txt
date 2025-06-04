cmake_minimum_required(VERSION 3.27)
project(opengl_graphics VERSION 1.0.0 LANGUAGES CXX)

add_library(opengl_graphics STATIC)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.hpp" "src/**.cpp")
target_sources(opengl_graphics
    PRIVATE
        ${SOURCES}
)

target_include_directories(opengl_graphics 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add glad
add_subdirectory(third_party/glad)

# Link libraries
target_link_libraries(opengl_graphics 
    PRIVATE 
        engine::compile_options
        engine::modules
        third_party::glad
        third_party::glm
)

set_target_properties(opengl_graphics PROPERTIES FOLDER "engine/modules/graphics")
add_library(engine::modules::graphics ALIAS opengl_graphics)
