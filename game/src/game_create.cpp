#include <game.hpp>

namespace game_engine
{

std::shared_ptr<game_engine::Game> Game::Create()
{
    return std::make_shared<::Game>();
}

} // namespace game_engine
