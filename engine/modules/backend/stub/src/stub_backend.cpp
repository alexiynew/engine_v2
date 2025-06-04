#include "stub_backend.hpp"

namespace game_engine::backend
{

StubBackend::StubBackend() = default;

StubBackend::~StubBackend() = default;

#pragma region IBackendModule implementation

bool StubBackend::Init(const GameSettings& /*settings*/) noexcept
{
    return true;
}

void StubBackend::Shutdown() noexcept
{}

void StubBackend::AttachBackendObserver(IBackendObserver& observer)
{
    m_observers.push_front(observer);
}

void StubBackend::DetachBackendObserver(const IBackendObserver& observer)
{
    m_observers.remove_if([&observer](const RefObserver& obj) { return &obj.get() == &observer; });
}

void StubBackend::PollEvents() const
{
    m_frames_count++;
    if (m_frames_count >= m_target_frames_count) {
        for (auto observer : m_observers) {
            observer.get().OnEvent(WindowCloseEvent{});
        }
    }
}

void StubBackend::MakeContextCurrent() const
{}

void StubBackend::DropCurrentContext() const
{}

void StubBackend::SwapBuffers() const
{}

} // namespace game_engine::backend
