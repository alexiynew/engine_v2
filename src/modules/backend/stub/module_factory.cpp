#include <module_factory.hpp>
#include <stub/stub_backend.hpp>

namespace game_engine
{

template <>
void ModuleFactory<backend::Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<backend::StubBackend>(); });
}

} // namespace game_engine
