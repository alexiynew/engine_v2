#pragma once

#include <modules/backend/backend_module.hpp>

namespace game_engine::backend
{

class StubBackend final : public IBackendModule
{
public:

    StubBackend();
    ~StubBackend() override;

    // IBackendModule implementation
    bool Init(const GameSettings& settings) noexcept override;
    void Shutdown() noexcept override;

    void AttachBackendObserver(IBackendObserver& observer) override;
    void DetachBackendObserver(const IBackendObserver& observer) override;

    void PollEvents() const override;

    void MakeContextCurrent() const override;
    void DropCurrentContext() const override;
    void SwapBuffers() const override;

private:

    mutable int m_frames_count = 0;
    int m_target_frames_count  = 100;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
