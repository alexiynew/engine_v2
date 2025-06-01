
#include "render_context_impl.hpp"

#include <modules/backend/backend.hpp>

namespace game_engine
{

RenderContextImpl::RenderContextImpl(std::shared_ptr<backend::IBackendModule> backend)
    : m_backend(std::move(backend))
{}

RenderContextImpl::~RenderContextImpl() = default;

#pragma region IRenderContext implementation

void RenderContextImpl::MakeCurrent() const
{
    m_backend->MakeContextCurrent();
}

void RenderContextImpl::DropCurrent() const
{
    m_backend->DropCurrentContext();
}

void RenderContextImpl::SwapBuffers() const
{
    m_backend->SwapBuffers();
}

#pragma endregion

} // namespace game_engine
