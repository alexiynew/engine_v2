#include <modules/module_factory.hpp>
#include <modules/render_context.hpp>

#include <opengl_renderer.hpp>

namespace game_engine
{

template <>
void ModuleFactory<graphics::Renderer>::RegisterModule()
{
    RegisterCreator([](std::shared_ptr<const RenderContext> context) {
        return std::make_shared<graphics::OpenGLRenderer>(std::move(context));
    });
}

} // namespace game_engine
