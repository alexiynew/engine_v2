target_sources(backend
    PRIVATE
        glfw/glfw_backend.cpp
        glfw/glfw_backend.hpp
        glfw/glfw_keyboard.cpp
        glfw/glfw_keyboard.hpp
)

include(libs/glfw)
target_link_libraries(backend PRIVATE libs::glfw)
