#include <glfw_backend.hpp>
#include <modules/module_factory.hpp>

namespace game_engine
{

template <>
void ModuleFactory<Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<backend::GLFWBackend>(); });
}

} // namespace game_engine
