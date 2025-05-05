#include <modules/module_factory.hpp>
#include <stub_backend.hpp>

namespace game_engine
{

template <>
void ModuleFactory<Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<backend::StubBackend>(); });
}

} // namespace game_engine
