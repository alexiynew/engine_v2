#pragma once

#include <game_engine/game.hpp>

class GameStub final : public game_engine::Game
{
public:
    GameStub(game_engine::core::Engine& engine);
    ~GameStub();

    void on_initialize() override;
    void on_update(std::chrono::nanoseconds elapsed_time) override;
    void on_draw() override;
    void on_shutdown() override;

    void on_keyboard_input_event(const game_engine::KeyboardInputEvent& event) override;

    bool on_should_close() override;

private:
    game_engine::core::Engine& m_engine;

    std::size_t m_updates_count = 0;
    std::size_t m_frames_count  = 0;
};
