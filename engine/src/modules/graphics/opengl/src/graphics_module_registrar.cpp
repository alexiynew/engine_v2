#include <modules/module_registrar.hpp>
#include <modules/render_context.hpp>

#include <opengl_renderer.hpp>

namespace game_engine
{

std::shared_ptr<graphics::Renderer> ModuleRegistrar<graphics::Renderer>::Create(
std::shared_ptr<const RenderContext> context)
{
    return std::make_shared<graphics::OpenGLRenderer>(std::move(context));
}

} // namespace game_engine
