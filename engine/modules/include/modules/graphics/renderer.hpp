#pragma once

#include <memory>

#include <engine/graphics/mesh.hpp>
#include <engine/graphics/property.hpp>
#include <engine/graphics/shader.hpp>

#include <modules/render_context.hpp>

namespace game_engine::graphics
{

struct RenderCommand
{
    std::shared_ptr<IMesh> mesh;
    std::shared_ptr<IShader> shader;
    std::vector<Property> properties;
    uint32_t instance_count = 1;
};

class IRendererModule
{
public:
    virtual ~IRendererModule() = default;

    static std::shared_ptr<IRendererModule> Create();

    virtual bool Init(std::shared_ptr<const IRenderContext> context) noexcept = 0;
    virtual void Shutdown() noexcept                                          = 0;

    virtual std::shared_ptr<IShader> CreateShader() = 0;
    virtual std::shared_ptr<IMesh> CreateMesh()     = 0;

    virtual void AddRenderCommand(const RenderCommand& command) = 0;
    virtual void ClearRenderCommands()                          = 0;
    virtual void ExecuteRenderCommands()                        = 0;
};

} // namespace game_engine::graphics
