#pragma once

#include <memory>

#include <engine/graphics/renderer.hpp>

#include <graphics/render_command.hpp>

// Forward declarations
namespace game_engine
{
class ModuleLocator;
class RenderContextImpl;

namespace graphics
{
class IRendererModule;
} // namespace graphics
} // namespace game_engine

namespace game_engine
{

class RendererImpl final : public IRenderer
{
public:

    explicit RendererImpl(const ModuleLocator& locator);
    ~RendererImpl() override;

    RendererImpl(const RendererImpl&) = delete;
    RendererImpl(RendererImpl&&)      = default;

    RendererImpl& operator=(const RendererImpl&) = delete;
    RendererImpl& operator=(RendererImpl&&)      = default;

    // IRenderer implementation
    void Render(const std::shared_ptr<IMesh>& mesh,
    const std::shared_ptr<IShader>& shader,
    const std::vector<Property>& properties) override;

    void Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material) override;

    // RendererImpl methods
    bool Init() noexcept;
    void Shutdown() noexcept;

    void AddRenderCommand(const RenderCommand& command);
    void ClearRenderCommands();
    void ExecuteRenderCommands();

private:

    std::shared_ptr<RenderContextImpl> m_context;
    std::shared_ptr<graphics::IRendererModule> m_renderer_module;
};

} // namespace game_engine
