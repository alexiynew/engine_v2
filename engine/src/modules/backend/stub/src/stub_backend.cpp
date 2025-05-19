#include "stub_backend.hpp"

namespace game_engine::backend
{

std::shared_ptr<IBackend> CreateBackendInstance()
{
    return std::make_shared<StubBackend>();
}

StubBackend::StubBackend() = default;

StubBackend::~StubBackend() = default;

bool StubBackend::Init(const GameSettings&) noexcept
{
    return true;
}

void StubBackend::Shutdown() noexcept
{}

void StubBackend::PollEvents()
{
    m_frames_count++;
    if (m_frames_count >= m_target_frames_count) {
        for (auto observer : m_observers) {
            observer.get().OnEvent(WindowCloseEvent{});
        }
    }
}

std::shared_ptr<const IRenderContext> StubBackend::GetRenderContext() const
{
    return shared_from_this();
}

void StubBackend::AttachBackendObserver(IBackendObserver& observer)
{
    m_observers.push_front(observer);
}

void StubBackend::DetachBackendObserver(const IBackendObserver& observer)
{
    m_observers.remove_if([&observer](const RefObserver& obj) { return &obj.get() == &observer; });
}

void StubBackend::MakeCurrent() const
{}

void StubBackend::DropCurrent() const
{}

void StubBackend::SwapBuffers() const
{}

} // namespace game_engine::backend
