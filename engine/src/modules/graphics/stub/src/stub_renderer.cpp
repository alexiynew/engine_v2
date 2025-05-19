#include "stub_renderer.hpp"

#include <stub_mesh.hpp>
#include <stub_shader.hpp>

namespace game_engine::graphics
{

StubRenderer::StubRenderer()
{}

StubRenderer::~StubRenderer() = default;

bool StubRenderer::init(std::shared_ptr<const IRenderContext>) noexcept
{
    return true;
}

void StubRenderer::shutdown() noexcept
{}

std::shared_ptr<graphics::IShader> StubRenderer::createShader()
{
    return std::make_shared<StubShader>();
}

std::shared_ptr<graphics::IMesh> StubRenderer::createMesh()
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

