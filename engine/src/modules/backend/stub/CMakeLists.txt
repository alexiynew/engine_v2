cmake_minimum_required(VERSION 3.27)
project(stub_backend VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_backend STATIC)

target_sources(stub_backend
    PRIVATE
        src/backend_create.cpp
        src/stub_backend.hpp
        src/stub_backend.cpp
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
