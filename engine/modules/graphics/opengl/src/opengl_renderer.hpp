#pragma once

#include <unordered_map>
#include <vector>

#include <modules/graphics/renderer_module.hpp>

namespace game_engine::graphics
{

class OpenGLMesh;
class OpenGLShader;
class OpenGLTexture;

class OpenGLRenderer final : public IRendererModule
{
public:

    OpenGLRenderer();
    ~OpenGLRenderer() override;

    OpenGLRenderer(const OpenGLRenderer&)  = delete;
    OpenGLRenderer(OpenGLRenderer&& other) = delete;

    OpenGLRenderer& operator=(const OpenGLRenderer&)  = delete;
    OpenGLRenderer& operator=(OpenGLRenderer&& other) = delete;

    // IRendererModule
    bool Init() override;
    void Shutdown() noexcept override;

    bool Load(const std::shared_ptr<IMesh>& mesh) override;
    bool Load(const std::shared_ptr<IShader>& shader) override;
    bool Load(const std::shared_ptr<ITexture>& texture) override;

    void Execute(const BeginFrameCommand& command) override;
    void Execute(const EndFrameCommand& command) override;
    void Execute(const RenderCommand& command) override;

private:

    std::unordered_map<ResourceId, OpenGLMesh> m_meshes;
    std::unordered_map<ResourceId, OpenGLShader> m_shaders;
    std::unordered_map<ResourceId, OpenGLTexture> m_textures;
};

} // namespace game_engine::graphics
