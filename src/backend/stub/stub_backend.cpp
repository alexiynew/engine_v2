#include "stub_backend.hpp"

namespace game_engine::backend
{

std::shared_ptr<Backend> createBackendInstance()
{
    return std::make_shared<StubBackend>();
}

StubBackend::StubBackend() = default;

StubBackend::~StubBackend() = default;

bool StubBackend::initialize()
{
    return true;
}

void StubBackend::shutdown()
{}

void StubBackend::pollEvents()
{
    if (m_framesCount <= m_targetFramesCount) {
        notify(WindowCloseEvent{});
    }
}

void StubBackend::beginFrame()
{}

void StubBackend::endFrame()
{
    m_framesCount++;
}

core::MeshId StubBackend::loadMesh(const core::Mesh&)
{
    return 0;
}

void StubBackend::renderMesh(core::MeshId)
{}

} // namespace game_engine::backend
