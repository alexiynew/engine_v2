#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <engine/common_types.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

class OpenGLMesh final
{
public:
    explicit OpenGLMesh(std::shared_ptr<OpenGLRenderer> render_thread) noexcept;
    ~OpenGLMesh();

    OpenGLMesh(const OpenGLMesh&) = delete;
    OpenGLMesh(OpenGLMesh&& other) noexcept;

    OpenGLMesh& operator=(const OpenGLMesh&) = delete;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;

    //void SetMeshData(const MeshData& data);
    //bool LoadToGpu();
    //void Clear() noexcept;
    //bool IsValid() const noexcept;

    //void Render() const;

private:
    friend void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept;

    bool LoadToGpuImpl();

    std::shared_ptr<OpenGLRenderer> m_renderer;

    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    // MeshData m_data = {};
};

} // namespace game_engine::graphics
