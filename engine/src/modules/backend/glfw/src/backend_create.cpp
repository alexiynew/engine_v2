#include <glfw_backend.hpp>

namespace game_engine::backend
{

std::shared_ptr<Backend> Backend::Create()
{
    return std::make_shared<GLFWBackend>();
}

} // namespace game_engine::backend
