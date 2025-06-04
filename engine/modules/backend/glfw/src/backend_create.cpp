#include <glfw_backend.hpp>

namespace game_engine::backend
{

std::shared_ptr<IBackendModule> IBackendModule::Create()
{
    return std::make_shared<GLFWBackend>();
}

} // namespace game_engine::backend
