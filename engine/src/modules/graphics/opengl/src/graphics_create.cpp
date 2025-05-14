#include <modules/render_context.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

std::shared_ptr<Renderer> Renderer::Create()
{
    return std::make_shared<OpenGLRenderer>();
}

} // namespace game_engine::graphics
