#pragma once

#include <memory>

#include <modules/graphics/render_context.hpp>

namespace game_engine::backend
{
class IBackendModule;
} // namespace game_engine::backend

namespace game_engine
{

class RenderContextImpl final : public graphics::IRenderContext
{
public:
    explicit RenderContextImpl(std::shared_ptr<backend::IBackendModule> backend);
    ~RenderContextImpl() override;

    RenderContextImpl(const RenderContextImpl&) = delete;
    RenderContextImpl(RenderContextImpl&&)      = default;

    RenderContextImpl& operator=(const RenderContextImpl&) = delete;
    RenderContextImpl& operator=(RenderContextImpl&&)      = default;

    // IRenderContext implementation
    void MakeCurrent() const override;
    void DropCurrent() const override;
    void SwapBuffers() const override;

private:
    std::shared_ptr<backend::IBackendModule> m_backend;
};

} // namespace game_engine
