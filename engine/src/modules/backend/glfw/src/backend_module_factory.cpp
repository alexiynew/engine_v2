#include <modules/module_factory.hpp>

#include <glfw_backend.hpp>

namespace game_engine
{

template <>
void ModuleFactory<backend::Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<backend::GLFWBackend>(); });
}

} // namespace game_engine
