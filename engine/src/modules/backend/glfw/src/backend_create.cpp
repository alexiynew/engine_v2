#include <glfw_backend.hpp>

namespace game_engine::backend
{

std::shared_ptr<backend::Backend> Backend::Create()
{
    return std::make_shared<backend::GLFWBackend>();
}

} // namespace game_engine::backend
