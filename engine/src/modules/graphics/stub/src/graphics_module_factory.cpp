#include <modules/module_factory.hpp>
#include <modules/render_context.hpp>

#include <stub_renderer.hpp>

namespace game_engine
{

template <>
void ModuleFactory<graphics::Renderer>::RegisterModule()
{
    RegisterCreator([](std::shared_ptr<RenderContext> context) {
        return std::make_shared<graphics::StubRenderer>(std::move(context));
    });
}

} // namespace game_engine
