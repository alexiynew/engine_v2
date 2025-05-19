#pragma once

#include <modules/graphics/renderer.hpp>
#include <modules/render_context.hpp>

namespace game_engine::graphics
{

class StubRenderer final : public IRenderer
{
public:
    StubRenderer();
    ~StubRenderer() override;

    // Renderer
    bool Init(std::shared_ptr<const IRenderContext>) noexcept override;
    void Shutdown() noexcept override;

    std::shared_ptr<graphics::IShader> CreateShader() override;
    std::shared_ptr<graphics::IMesh> CreateMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;
};

} // namespace game_engine::graphics
