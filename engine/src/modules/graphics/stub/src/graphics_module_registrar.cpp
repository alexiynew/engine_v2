#include <engine/module_registrar.hpp>

#include <modules/render_context.hpp>

#include <stub_renderer.hpp>

namespace game_engine
{

template <>
void ModuleRegistrar::RegisterModule<graphics::Renderer>(ModuleFactory& factory)
{
    factory.registerCreator<graphics::Renderer>([](std::shared_ptr<const RenderContext> context) {
        return std::make_shared<graphics::StubRenderer>(std::move(context));
    });
}

} // namespace game_engine
