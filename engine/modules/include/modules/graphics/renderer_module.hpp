#pragma once

#include <memory>

#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>
#include <engine/graphics/texture.hpp>

#include <modules/graphics/render_command.hpp>

namespace game_engine::graphics
{

class IRendererModule
{
public:

    virtual ~IRendererModule() = default;

    static std::shared_ptr<IRendererModule> Create();

    virtual bool Init()              = 0;
    virtual void Shutdown() noexcept = 0;

    virtual bool Load(const std::shared_ptr<IMesh>& mesh)       = 0;
    virtual bool Load(const std::shared_ptr<IShader>& shader)   = 0;
    virtual bool Load(const std::shared_ptr<ITexture>& texture) = 0;

    virtual void Execute(const BeginFrameCommand& command) = 0;
    virtual void Execute(const EndFrameCommand& command)   = 0;
    virtual void Execute(const RenderCommand& command)     = 0;
};

} // namespace game_engine::graphics
