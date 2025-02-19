add_library(glfw_backend STATIC)

target_sources(glfw_backend
    PRIVATE
        glfw_backend.hpp
        glfw_backend.cpp
        glfw_backend_context.hpp
        glfw_backend_context.cpp
        glfw_keyboard.hpp
        glfw_keyboard.cpp
        opengl_shader.hpp
        opengl_shader.cpp
        opengl_utils.hpp
        opengl_utils.cpp
)

set_target_properties(glfw_backend PROPERTIES FOLDER "backend")
target_include_directories(glfw_backend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(glfw_backend PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(libs/glfw)
target_link_libraries(glfw_backend PUBLIC Libs::glfw)

include(libs/glm)
target_link_libraries(glfw_backend PUBLIC Libs::glm)


add_subdirectory(glad)
target_link_libraries(glfw_backend PRIVATE glad)
target_include_directories(glfw_backend PRIVATE glad/include)
