#include "game.hpp"

#include <iostream>

namespace game_engine
{

void Game::initialize() {
    std::cout << "Game::initialize" << std::endl;
}

void Game::run() {
    std::cout << "Game::run" << std::endl;
}

void Game::shutdown() {
    std::cout << "Game::shutdown" << std::endl;
}

} // namespace game_engine
