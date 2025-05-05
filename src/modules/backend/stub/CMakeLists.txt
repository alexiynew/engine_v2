cmake_minimum_required(VERSION 3.27)
project(stub_backend VERSION 1.0.0 LANGUAGES CXX)

add_library(stub_backend STATIC)

target_sources(stub_backend
    PRIVATE
        src/backend_module_factory.cpp
        src/stub_backend.hpp
        src/stub_backend.cpp
)

target_include_directories(stub_backend 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/modules/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(stub_backend 
    PRIVATE 
        engine::compile_options
)

set_target_properties(stub_backend PROPERTIES FOLDER "modules/backend")

add_library(modules::backend ALIAS stub_backend)
