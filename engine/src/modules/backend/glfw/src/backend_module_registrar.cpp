#include <modules/module_registrar.hpp>

#include <glfw_backend.hpp>

namespace game_engine
{

std::shared_ptr<backend::Backend> ModuleRegistrar<backend::Backend>::Create()
{
    return std::make_shared<backend::GLFWBackend>();
}

} // namespace game_engine
