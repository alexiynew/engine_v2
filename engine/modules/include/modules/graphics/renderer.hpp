#pragma once

#include <memory>

namespace game_engine::graphics
{

class IRenderContext;

class IRendererModule
{
public:

    virtual ~IRendererModule() = default;

    static std::shared_ptr<IRendererModule> Create();

    virtual bool Init(std::shared_ptr<IRenderContext> context) noexcept = 0;
    virtual void Shutdown() noexcept                                    = 0;

    // virtual std::shared_ptr<IShader> CreateShader() = 0;
    // virtual std::shared_ptr<IMesh> CreateMesh()     = 0;

    // virtual void AddRenderCommand(const RenderCommand& command) = 0;
    // virtual void ClearRenderCommands()                          = 0;
    // virtual void ExecuteRenderCommands()                        = 0;
};

} // namespace game_engine::graphics
