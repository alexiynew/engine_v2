cmake_minimum_required(VERSION 3.27)
project(stub_backend VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_backend STATIC)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.hpp" "src/**.cpp")
target_sources(stub_backend
    PRIVATE
        ${SOURCES}
)

target_include_directories(stub_backend 
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(stub_backend 
    PRIVATE 
        engine::compile_options
        engine::modules
)

set_target_properties(stub_backend PROPERTIES FOLDER "engine/modules/backend")
add_library(engine::modules::backend ALIAS stub_backend)
