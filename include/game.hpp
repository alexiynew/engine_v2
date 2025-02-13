#pragma once

#include <memory>

#include "core/engine.hpp"

namespace game_engine
{

std::unique_ptr<game_engine::Game> create_game_instance();

class Game
{
public:
    Game();
    virtual ~Game();

    virtual void on_initialize();
    virtual void on_update();
    virtual void on_draw();
    virtual void on_shutdown();

private:
    std::unique_ptr<core::Engine> m_engine;
};

} // namespace game_engine

