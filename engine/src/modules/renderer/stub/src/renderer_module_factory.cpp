#include <modules/module_factory.hpp>
#include <modules/render_context.hpp>
#include <stub_renderer.hpp>

namespace game_engine
{

template <>
void ModuleFactory<Renderer>::RegisterModule()
{
    RegisterCreator([](std::shared_ptr<RenderContext> context) {
        return std::make_shared<renderer::StubRenderer>(std::move(context));
    });
}

} // namespace game_engine
