#pragma once

#include <modules/backend/backend.hpp>
#include <modules/render_context.hpp>

namespace game_engine::backend
{

class StubBackend final
    : public IBackend
    , public IRenderContext
    , public std::enable_shared_from_this<StubBackend>
{
public:
    StubBackend();
    ~StubBackend() override;

    // IBackend
    bool Init(const GameSettings&) noexcept override;
    void Shutdown() noexcept override;

    void PollEvents() override;
    std::shared_ptr<IRenderContext> GetRenderContext() const override;

    void AttachBackendObserver(IBackendObserver& observer) override;
    void DetachBackendObserver(const IBackendObserver& observer) override;

    // renderer::IRenderContext
    void MakeCurrent() const override;
    void DropCurrent() const override;
    void SwapBuffers() const override;

private:
    int m_frames_count        = 0;
    int m_target_frames_count = 100;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
