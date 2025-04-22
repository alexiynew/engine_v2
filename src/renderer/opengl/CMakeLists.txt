target_sources(renderer
    PRIVATE
        opengl/opengl_mesh.cpp
        opengl/opengl_mesh.hpp
        opengl/opengl_shader.cpp
        opengl/opengl_shader.hpp
        opengl/opengl_utils.cpp
        opengl/opengl_utils.hpp
        opengl/opengl_renderer.cpp
        opengl/opengl_renderer.hpp
)

add_subdirectory(opengl/glad)
target_link_libraries(renderer PRIVATE glad)
target_include_directories(renderer PRIVATE glad/include)
