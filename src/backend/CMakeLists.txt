# Backend library
add_library(backend STATIC)

target_sources(backend
    PUBLIC
        backend.hpp
    PRIVATE
        backend.cpp
)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(backend PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_target_properties(backend PROPERTIES FOLDER "backend")

add_subdirectory(glfw)

target_link_libraries(backend PUBLIC glfw_backend)

add_library(engine::backend ALIAS backend)
