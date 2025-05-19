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

    void pollEvents() override;
    std::shared_ptr<const IRenderContext> getRenderContext() const override;

    void attachBackendObserver(IBackendObserver& observer) override;
    void detachBackendObserver(const IBackendObserver& observer) override;

    // renderer::IRenderContext
    void makeCurrent() const override;
    void dropCurrent() const override;
    void swapBuffers() const override;

private:
    int m_framesCount       = 0;
    int m_targetFramesCount = 100;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
