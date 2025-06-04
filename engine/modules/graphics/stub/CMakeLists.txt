cmake_minimum_required(VERSION 3.27)
project(stub_graphics VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_graphics STATIC)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.hpp" "src/**.cpp")
target_sources(stub_graphics
    PRIVATE
        ${SOURCES}
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
