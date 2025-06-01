#include "renderer_impl.hpp"

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>
#include <modules/module_locator.hpp>

#include <graphics/render_context_impl.hpp>

namespace game_engine
{

RendererImpl::RendererImpl(const ModuleLocator& locator)
    : m_context(std::make_shared<RenderContextImpl>(locator.Get<backend::IBackendModule>()))
    , m_renderer_module(locator.Get<graphics::IRendererModule>())
{}

RendererImpl::~RendererImpl() = default;

#pragma region IRenderer implementation

void RendererImpl::Render(const std::shared_ptr<IMesh>& mesh,
                          const std::shared_ptr<IShader>& shader,
                          const std::vector<Property>& properties)
{}

void RendererImpl::Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material)
{}

#pragma endregion

#pragma region RendererImpl methods

bool RendererImpl::Init() noexcept
{
    return m_renderer_module->Init(m_context);
}

void RendererImpl::Shutdown() noexcept
{
    m_renderer_module->Shutdown();
}

void RendererImpl::AddRenderCommand(const RenderCommand& command)
{}

void RendererImpl::ClearRenderCommands()
{}

void RendererImpl::ExecuteRenderCommands()
{}

#pragma endregion

} // namespace game_engine
