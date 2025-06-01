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
    bool Init(std::shared_ptr<IRenderContext>) noexcept override;
    void Shutdown() noexcept override;

    std::shared_ptr<graphics::IShader> CreateShader() override;
    std::shared_ptr<graphics::IMesh> CreateMesh() override;

    void AddRenderCommand(const RenderCommand& command) override;
    void ClearRenderCommands() override;
    void ExecuteRenderCommands() override;
};

} // namespace game_engine::graphics
