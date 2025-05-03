target_sources(renderer
    PRIVATE
        src/opengl/module_factory.cpp
        src/opengl/opengl_mesh.cpp
        src/opengl/opengl_mesh.hpp
        src/opengl/opengl_shader.cpp
        src/opengl/opengl_shader.hpp
        src/opengl/opengl_utils.cpp
        src/opengl/opengl_utils.hpp
        src/opengl/opengl_renderer.cpp
        src/opengl/opengl_renderer.hpp
)

add_subdirectory(src/opengl/glad)
target_link_libraries(renderer PRIVATE glad)
target_include_directories(renderer PRIVATE glad/include)
