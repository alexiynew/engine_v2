#include <backend/backend.hpp>
#include <core/engine_impl.hpp>

int main()
{
    auto backend = game_engine::backend::createBackendInstance();
    auto engine  = std::make_shared<game_engine::core::EngineImpl>(backend);
    auto game    = game_engine::createGameInstance(*engine);

    engine->setGameInstance(game);

    const int returnCode = engine->run();

    return returnCode;
}
