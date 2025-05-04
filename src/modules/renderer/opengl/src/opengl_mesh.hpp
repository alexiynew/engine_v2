#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/mesh.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::renderer
{

class OpenGLMesh final : public core::Mesh
{
public:
    explicit OpenGLMesh(std::shared_ptr<OpenGLRenderer> renderThread) noexcept;
    ~OpenGLMesh() override;

    OpenGLMesh(const OpenGLMesh&) = delete;
    OpenGLMesh(OpenGLMesh&& other) noexcept;

    OpenGLMesh& operator=(const OpenGLMesh&) = delete;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;

    // core::Mesh
    virtual void setMeshData(const core::MeshData& data) override;
    virtual void flush() override;
    virtual void clear() noexcept override;
    virtual bool isValid() const noexcept override;

    void render() const;

private:
    friend void swap(OpenGLMesh& a, OpenGLMesh& b);

    bool loadToGPU();

    std::shared_ptr<OpenGLRenderer> m_renderer;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    core::MeshData m_data = {};
};

} // namespace game_engine::renderer
