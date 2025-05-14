#pragma once

#include <memory>

#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>

#include <modules/module_registrar.hpp>
#include <modules/render_context.hpp>

namespace game_engine::graphics
{

struct RenderCommand
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    std::vector<Uniform> uniforms;
    uint32_t instanceCount = 1;
};

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual bool initialize() noexcept = 0;
    virtual void shutdown() noexcept   = 0;

    virtual std::shared_ptr<Shader> createShader() = 0;
    virtual std::shared_ptr<Mesh> createMesh()     = 0;

    virtual void addRenderCommand(const RenderCommand& command) = 0;
    virtual void clearRenderCommands()                          = 0;
    virtual void executeRenderCommands()                        = 0;
};

} // namespace game_engine::graphics

namespace game_engine
{

template <>
struct ModuleRegistrar<graphics::Renderer>
{
    static std::shared_ptr<graphics::Renderer> Create(std::shared_ptr<const RenderContext> context);
};

} // namespace game_engine
