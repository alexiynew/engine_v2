#pragma once

#include <memory>

#include <game_engine/core/mesh.hpp>
#include <game_engine/core/shader.hpp>

namespace game_engine::renderer
{

class RendererContext
{
public:
    virtual ~RendererContext() = default;

    virtual void makeCurrent() = 0;
    virtual void dropCurrent() = 0;
    virtual void swapBuffers() = 0;
};

struct RenderCommand
{
    std::shared_ptr<core::Mesh> mesh;
    std::shared_ptr<core::Shader> shader;
    std::vector<core::Uniform> uniforms;
    uint32_t instanceCount = 1;
};

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual bool initialize() noexcept = 0;
    virtual void shutdown() noexcept   = 0;

    virtual std::shared_ptr<core::Shader> createShader() = 0;
    virtual std::shared_ptr<core::Mesh> createMesh()     = 0;

    virtual void addRenderCommand(const RenderCommand& command) = 0;
    virtual void clearRenderCommands()                          = 0;
    virtual void executeRenderCommands()                        = 0;
};

std::shared_ptr<Renderer> createRendererInstance(std::shared_ptr<RendererContext> context);

} // namespace game_engine::renderer
