# Backend library
add_library(backend STATIC)

target_sources(backend
    PUBLIC
        backend.hpp
    PRIVATE
        backend.cpp
)

target_include_directories(backend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(backend PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(backend PROPERTIES FOLDER "backend")

add_subdirectory(glfw)

target_link_libraries(backend PRIVATE glfw_backend)
target_link_libraries(backend PRIVATE engine::compile_options)

add_library(engine::backend ALIAS backend)
