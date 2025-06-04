#pragma once

#include <modules/graphics/renderer_module.hpp>

namespace game_engine::graphics
{

class StubRenderer final : public IRendererModule
{
public:

    StubRenderer();
    ~StubRenderer() override;

    // IRendererModule implementation
    bool Init() override;
    void Shutdown() noexcept override;

    bool Load(const std::shared_ptr<IMesh>& mesh) override;
    bool Load(const std::shared_ptr<IShader>& shader) override;
    bool Load(const std::shared_ptr<ITexture>& texture) override;

    void Unload(ResourceType type, ResourceId id) override;
    void UnloadAll() override;

    void Execute(const BeginFrameCommand& command) override;
    void Execute(const EndFrameCommand& command) override;
    void Execute(const RenderCommand& command) override;
};

} // namespace game_engine::graphics
