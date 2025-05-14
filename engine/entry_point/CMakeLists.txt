cmake_minimum_required(VERSION 3.27)
project(entry_point VERSION 1.0.0 LANGUAGES CXX)

add_library(entry_point OBJECT)

target_sources(entry_point
    PRIVATE
        src/entry_point.cpp
)

target_include_directories(entry_point
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/engine/src
)

target_link_libraries(entry_point
    PRIVATE
        engine
        engine::compile_options
        engine::modules
)   

# Setup entry point on windows
if(USE_WINMAIN)
    set_target_properties(entry_point PROPERTIES WIN32_EXECUTABLE ON)
    target_compile_definitions(entry_point PRIVATE USE_WINMAIN_ENTRY)
endif()

set_target_properties(entry_point PROPERTIES FOLDER "engine")
add_library(engine::entry_point ALIAS entry_point)
