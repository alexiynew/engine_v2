#include <glfw/glfw_backend.hpp>
#include <module_factory.hpp>

namespace game_engine
{

template <>
void ModuleFactory<backend::Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<backend::GLFWBackend>(); });
}

} // namespace game_engine
