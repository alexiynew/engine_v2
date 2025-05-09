#pragma once

#include <modules/graphics/renderer.hpp>
#include <modules/render_context.hpp>

namespace game_engine::graphics
{

class StubRenderer final : public Renderer
{
public:
    explicit StubRenderer(std::shared_ptr<RenderContext> context);
    ~StubRenderer() override;

    // Renderer
    bool initialize() noexcept override;
    void shutdown() noexcept override;

    std::shared_ptr<graphics::Shader> createShader() override;
    std::shared_ptr<graphics::Mesh> createMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;
};

} // namespace game_engine::graphics
