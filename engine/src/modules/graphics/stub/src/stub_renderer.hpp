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
    bool init(std::shared_ptr<const IRenderContext>) noexcept override;
    void shutdown() noexcept override;

    std::shared_ptr<graphics::IShader> createShader() override;
    std::shared_ptr<graphics::IMesh> createMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;
};

} // namespace game_engine::graphics
