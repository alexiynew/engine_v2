
add_library(backend STATIC)
target_sources(backend
    PUBLIC
        backend.hpp
    PRIVATE
        glfw/glfw_backend.hpp
        glfw/glfw_backend.cpp
)

add_library(entry_point OBJECT)
target_sources(entry_point 
    PUBLIC
        glfw/entry_point.cpp
)

target_include_directories(entry_point PRIVATE .)

target_include_directories(backend PRIVATE .)

include(libs/glfw)

add_subdirectory(glad)

target_link_libraries(backend PUBLIC Libs::glfw)
target_link_libraries(backend PUBLIC entry_point)
target_link_libraries(backend PUBLIC glad)

