#pragma once

#include <modules/render_context.hpp>
#include <modules/renderer.hpp>

namespace game_engine::renderer
{

class StubRenderer final : public Renderer
{
public:
    explicit StubRenderer(std::shared_ptr<RenderContext> context);
    ~StubRenderer() override;

    // Renderer
    bool initialize() noexcept override;
    void shutdown() noexcept override;

    std::shared_ptr<core::Shader> createShader() override;
    std::shared_ptr<core::Mesh> createMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;
};

} // namespace game_engine::renderer
