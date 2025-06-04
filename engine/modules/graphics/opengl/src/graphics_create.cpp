#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

std::shared_ptr<IRendererModule> IRendererModule::Create()
{
    return std::make_shared<OpenGLRenderer>();
}

} // namespace game_engine::graphics
