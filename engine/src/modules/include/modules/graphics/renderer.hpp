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
    uint32_t instance_count = 1;
};

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    static std::shared_ptr<IRenderer> Create();

    virtual bool Init(std::shared_ptr<const IRenderContext> context) noexcept = 0;
    virtual void Shutdown() noexcept                                          = 0;

    virtual std::shared_ptr<IShader> CreateShader() = 0;
    virtual std::shared_ptr<IMesh> CreateMesh()     = 0;

    virtual void AddRenderCommand(const RenderCommand& command) = 0;
    virtual void ClearRenderCommands()                          = 0;
    virtual void ExecuteRenderCommands()                        = 0;
};

} // namespace game_engine::graphics
