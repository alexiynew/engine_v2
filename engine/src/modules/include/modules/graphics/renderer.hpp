#pragma once

#include <memory>

#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>

#include <modules/render_context.hpp>

namespace game_engine::graphics
{

struct RenderCommand
{
    std::shared_ptr<IMesh> mesh;
    std::shared_ptr<IShader> shader;
    std::vector<Uniform> uniforms;
    uint32_t instanceCount = 1;
};

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    static std::shared_ptr<IRenderer> Create();

    virtual bool init(std::shared_ptr<const IRenderContext> context) noexcept = 0;
    virtual void shutdown() noexcept                                          = 0;

    virtual std::shared_ptr<IShader> createShader() = 0;
    virtual std::shared_ptr<IMesh> createMesh()     = 0;

    virtual void addRenderCommand(const RenderCommand& command) = 0;
    virtual void clearRenderCommands()                          = 0;
    virtual void executeRenderCommands()                        = 0;
};

} // namespace game_engine::graphics
