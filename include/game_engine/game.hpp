#pragma once

#include <chrono>
#include <memory>

#include <game_engine/core/engine.hpp>
#include <game_engine/keyboard.hpp>

namespace game_engine
{

class Game
{
public:
    virtual ~Game() = default;

    virtual void on_initialize()                                  = 0;
    virtual void on_update(std::chrono::nanoseconds elapsed_time) = 0;
    virtual void on_draw()                                        = 0;
    virtual void on_shutdown()                                    = 0;

    virtual void on_keyboard_input_event(const KeyboardInputEvent& event) = 0;

    virtual bool on_should_close() = 0;
};

std::unique_ptr<Game> create_game_instance(core::Engine& engine);

} // namespace game_engine

