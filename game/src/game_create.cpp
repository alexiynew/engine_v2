#include <game.hpp>

namespace game_engine
{

std::shared_ptr<game_engine::Game> Game::Create(Engine& engine)
{
    return std::make_shared<::Game>(engine);
}

} // namespace game_engine
