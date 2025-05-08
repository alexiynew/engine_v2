#include "stub_renderer.hpp"

#include <stub_mesh.hpp>
#include <stub_shader.hpp>

namespace game_engine::renderer
{

std::shared_ptr<Renderer> createRendererInstance(std::shared_ptr<RenderContext> context)
{
    return std::make_shared<StubRenderer>(std::move(context));
}

StubRenderer::StubRenderer(std::shared_ptr<RenderContext>)
{}

StubRenderer::~StubRenderer() = default;

bool StubRenderer::initialize() noexcept
{
    return true;
}

void StubRenderer::shutdown() noexcept
{}

std::shared_ptr<core::Shader> StubRenderer::createShader()
{
    return std::make_shared<StubShader>();
}

std::shared_ptr<core::Mesh> StubRenderer::createMesh()
{
    return std::make_shared<StubMesh>();
}

void StubRenderer::addRenderCommand(const RenderCommand& command)
{}

void StubRenderer::clearRenderCommands()
{}

void StubRenderer::executeRenderCommands()
{}

} // namespace game_engine::renderer

