#pragma once

#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/mesh.hpp>

namespace game_engine::backend
{

class OpenGLMesh final : public core::Mesh
{
public:
    OpenGLMesh() noexcept;
    ~OpenGLMesh() noexcept;

    OpenGLMesh(const OpenGLMesh&) = delete;
    OpenGLMesh(OpenGLMesh&& other) noexcept;

    OpenGLMesh& operator=(const OpenGLMesh&) = delete;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;

    // core::Mesh
    virtual void setMeshData(const core::MeshData& data) override;
    virtual void flush() override;
    virtual void clear() override;
    virtual bool isValid() const override;

    void render() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    core::MeshData m_data;

    friend void swap(OpenGLMesh& a, OpenGLMesh& b);

    bool loadToGPU();
};

} // namespace game_engine::backend
