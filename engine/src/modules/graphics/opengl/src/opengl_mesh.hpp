#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <engine/common_types.hpp>
#include <engine/graphics/mesh.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

class OpenGLMesh final : public graphics::IMesh
{
public:
    explicit OpenGLMesh(std::shared_ptr<OpenGLRenderer> renderThread) noexcept;
    ~OpenGLMesh() override;

    OpenGLMesh(const OpenGLMesh&) = delete;
    OpenGLMesh(OpenGLMesh&& other) noexcept;

    OpenGLMesh& operator=(const OpenGLMesh&) = delete;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;

    // graphics::Mesh
    void SetMeshData(const graphics::MeshData& data) override;
    void Flush() override;
    void Clear() noexcept override;
    bool IsValid() const noexcept override;

    void Render() const;

private:
    friend void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept;

    bool loadToGPU();

    std::shared_ptr<OpenGLRenderer> m_renderer;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    graphics::MeshData m_data = {};
};

} // namespace game_engine::graphics
