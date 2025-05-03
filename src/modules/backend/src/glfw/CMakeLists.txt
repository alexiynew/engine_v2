target_sources(backend
    PRIVATE
        src/glfw/module_factory.cpp
        src/glfw/glfw_backend.cpp
        src/glfw/glfw_backend.hpp
        src/glfw/glfw_keyboard.cpp
        src/glfw/glfw_keyboard.hpp
)

include(libs/glfw)
target_link_libraries(backend PRIVATE libs::glfw)
