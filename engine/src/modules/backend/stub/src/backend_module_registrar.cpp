#include <engine/module_registrar.hpp>

#include <stub_backend.hpp>

namespace game_engine
{

template <>
void ModuleRegistrar::RegisterModule<backend::Backend>(ModuleFactory& factory)
{
    factory.registerCreator<backend::Backend>([]() { return std::make_shared<backend::StubBackend>(); });
}

} // namespace game_engine
