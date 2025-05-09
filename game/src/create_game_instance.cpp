#include <game.hpp>

// Implement game factory
namespace game_engine
{

std::shared_ptr<game_engine::Game> createGameInstance(game_engine::Engine& engine)
{
    return std::make_shared<::Game>(engine);
}

} // namespace game_engine

