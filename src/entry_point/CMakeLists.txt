# Entry point library
add_library(entry_point OBJECT)

target_sources(entry_point
    PRIVATE
        entry_point.cpp
)

target_include_directories(entry_point PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(entry_point PRIVATE ${CMAKE_SOURCE_DIR}/src)

set_target_properties(entry_point PROPERTIES FOLDER "entry_point")

add_library(engine::entry_point ALIAS entry_point)
