#include "stub_backend.hpp"

namespace game_engine::backend
{

std::shared_ptr<Backend> createBackendInstance()
{
    return std::make_shared<StubBackend>();
}

StubBackend::StubBackend() = default;

StubBackend::~StubBackend() = default;

bool StubBackend::initialize(const GameSettings&)
{
    return true;
}

void StubBackend::shutdown()
{}

void StubBackend::pollEvents()
{
    m_framesCount++;
    if (m_framesCount >= m_targetFramesCount) {
        for (auto observer : m_observers) {
            observer.get().onEvent(WindowCloseEvent{});
        }
    }
}

std::shared_ptr<RenderContext> StubBackend::getRenderContext()
{
    return shared_from_this();
}

void StubBackend::attachBackendObserver(BackendObserver& observer)
{
    m_observers.push_front(observer);
}

void StubBackend::detachBackendObserver(const BackendObserver& observer)
{
    m_observers.remove_if([&observer](const RefObserver& obj) { return &obj.get() == &observer; });
}

void StubBackend::makeCurrent()
{}

void StubBackend::dropCurrent()
{}

void StubBackend::swapBuffers()
{}

} // namespace game_engine::backend
