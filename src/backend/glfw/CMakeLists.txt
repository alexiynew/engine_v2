target_sources(backend
    PRIVATE
        glfw/glfw_backend.hpp
        glfw/glfw_backend.cpp
        glfw/glfw_backend_context.hpp
        glfw/glfw_backend_context.cpp
        glfw/glfw_keyboard.hpp
        glfw/glfw_keyboard.cpp
        glfw/opengl_shader.hpp
        glfw/opengl_shader.cpp
        glfw/opengl_utils.hpp
        glfw/opengl_utils.cpp
)

include(libs/glfw)
target_link_libraries(backend PRIVATE Libs::glfw)

add_subdirectory(glfw/glad)
target_link_libraries(backend PRIVATE glad)
target_include_directories(backend PRIVATE glad/include)
