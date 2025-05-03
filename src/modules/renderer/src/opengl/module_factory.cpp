#include <module_factory.hpp>
#include <modules/render_context.hpp>
#include <opengl/opengl_renderer.hpp>

namespace game_engine
{

template <>
void ModuleFactory<renderer::Renderer>::RegisterModule()
{
    RegisterCreator([](std::shared_ptr<RenderContext> context) {
        return std::make_shared<renderer::OpenGLRenderer>(std::move(context));
    });
}

} // namespace game_engine
