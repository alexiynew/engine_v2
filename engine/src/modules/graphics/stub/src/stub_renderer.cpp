#include "stub_renderer.hpp"

#include <stub_mesh.hpp>
#include <stub_shader.hpp>

namespace game_engine::graphics
{

StubRenderer::StubRenderer()
{}

StubRenderer::~StubRenderer() = default;

bool StubRenderer::Init(std::shared_ptr<const IRenderContext>) noexcept
{
    return true;
}

void StubRenderer::Shutdown() noexcept
{}

std::shared_ptr<graphics::IShader> StubRenderer::CreateShader()
{
    return std::make_shared<StubShader>();
}

std::shared_ptr<graphics::IMesh> StubRenderer::CreateMesh()
{
    return std::make_shared<StubMesh>();
}

void StubRenderer::addRenderCommand(const RenderCommand& command)
{}

void StubRenderer::clearRenderCommands()
{}

void StubRenderer::executeRenderCommands()
{}

} // namespace game_engine::graphics

