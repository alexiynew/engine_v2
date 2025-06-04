#include "stub_renderer.hpp"

namespace game_engine::graphics
{

StubRenderer::StubRenderer()
{}

StubRenderer::~StubRenderer() = default;

#pragma region IRendererModule implementation

bool StubRenderer::Init()
{
    return true;
}

void StubRenderer::Shutdown() noexcept
{}

bool StubRenderer::Load(const std::shared_ptr<IMesh>& mesh)
{
    return true;
}

bool StubRenderer::Load(const std::shared_ptr<IShader>& shader)
{
    return true;
}

bool StubRenderer::Load(const std::shared_ptr<ITexture>& texture)
{
    return true;
}

void StubRenderer::Unload(ResourceType type, ResourceId id)
{}

void StubRenderer::UnloadAll()
{}

void StubRenderer::Execute(const BeginFrameCommand& command)
{}

void StubRenderer::Execute(const EndFrameCommand& command)
{}

void StubRenderer::Execute(const RenderCommand& command)
{}

#pragma endregion

} // namespace game_engine::graphics

