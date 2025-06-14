#pragma once

#include <vector>

#include <engine/graphics/mesh.hpp>

#include <glad/glad.h>

namespace game_engine::graphics
{

class OpenGLMesh final
{
public:

    OpenGLMesh();
    ~OpenGLMesh();

    OpenGLMesh(const OpenGLMesh&) = delete;
    OpenGLMesh(OpenGLMesh&& other) noexcept;

    OpenGLMesh& operator=(const OpenGLMesh&) = delete;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;

    bool Load(const std::shared_ptr<IMesh>& mesh);
    bool IsValid() const noexcept;
    void Clear() noexcept;

    void Render() const;
    void RenderInstanced(GLsizei instance_count) const;

private:

    struct SubMesh
    {
        GLsizei indices_count;
        GLintptr indices_offset;
        GLenum indices_type;
        GLenum primitive_type;
    };

    friend void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept;

    std::vector<SubMesh> m_submeshes;

    GLint m_vertex_count = 0;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
};

} // namespace game_engine::graphics
