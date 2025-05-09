#pragma once

#include <modules/backend/backend.hpp>
#include <modules/render_context.hpp>

namespace game_engine::backend
{

class StubBackend final
    : public Backend
    , public RenderContext
    , public std::enable_shared_from_this<StubBackend>
{
public:
    StubBackend();
    ~StubBackend() override;

    // Backend
    bool initialize(const GameSettings&) override;
    void shutdown() override;
    void pollEvents() override;

    std::shared_ptr<RenderContext> getRenderContext() override;

    void attachBackendObserver(BackendObserver& observer) override;
    void detachBackendObserver(const BackendObserver& observer) override;

    // renderer::RenderContext
    void makeCurrent() override;
    void dropCurrent() override;
    void swapBuffers() override;

private:
    int m_framesCount       = 0;
    int m_targetFramesCount = 100;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
