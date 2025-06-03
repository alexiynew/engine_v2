#pragma once

#include <engine/graphics/mesh.hpp>

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

private:

    friend void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept;

    unsigned int m_primitive_type = 0;

    int m_indices_count = 0;

    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};

} // namespace game_engine::graphics
