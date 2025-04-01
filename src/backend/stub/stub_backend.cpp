#include "stub_backend.hpp"

#include <stub/stub_mesh.hpp>
#include <stub/stub_shader.hpp>

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
    if (m_framesCount >= m_targetFramesCount) {
        notify(WindowCloseEvent{});
    }
}

void StubBackend::beginFrame()
{}

void StubBackend::endFrame()
{
    m_framesCount++;
}

void StubBackend::applySettings(const GameSettings&)
{}

std::shared_ptr<core::Shader> StubBackend::createShader()
{
    return std::make_shared<StubShader>();
}

void StubBackend::useShader(const std::shared_ptr<core::Shader>&)
{}

std::shared_ptr<core::Mesh> StubBackend::createMesh()
{
    return std::make_shared<StubMesh>();
}

void StubBackend::render(const std::shared_ptr<core::Mesh>& mesh)
{}

} // namespace game_engine::backend
