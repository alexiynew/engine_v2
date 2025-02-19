#pragma once

#include <game_engine/game.hpp>

class GameStub final : public game_engine::Game
{
public:
    GameStub(game_engine::core::Engine& engine);
    ~GameStub();

    void onInitialize() override;
    void onUpdate(std::chrono::nanoseconds elapsedTime) override;
    void onDraw() override;
    void onShutdown() override;

    void onKeyboardInputEvent(const game_engine::KeyboardInputEvent& event) override;

    bool onShouldClose() override;

private:
    game_engine::core::Engine& m_engine;

    std::size_t m_updatesCount = 0;
    std::size_t m_framesCount  = 0;

    game_engine::core::MeshId m_meshId = 0;
};
