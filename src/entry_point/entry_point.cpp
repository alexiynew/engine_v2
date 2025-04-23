#include <backend/backend.hpp>
#include <core/engine_impl.hpp>
#include <renderer/renderer.hpp>

int main()
{
    auto engine = []() {
        auto backend  = game_engine::backend::createBackendInstance();
        auto renderer = game_engine::renderer::createRendererInstance(backend->getRendererContext());
        auto engine   = std::make_shared<game_engine::core::EngineImpl>(backend, renderer);
        auto game     = game_engine::createGameInstance(*engine);

        engine->setGameInstance(game);

        return engine;
    }();

    const int returnCode = engine->run();

    return returnCode;
}
