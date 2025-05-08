#pragma once

#include <game_engine/game.hpp>

class Game final : public game_engine::Game
{
public:
    explicit Game(game_engine::core::Engine& engine);
    ~Game() override;

    void onInitialize() override;
    void onUpdate(std::chrono::nanoseconds elapsedTime) override;
    void onDraw() override;
    void onShutdown() override;

    void onKeyboardInputEvent(const game_engine::KeyboardInputEvent& event) override;

    bool onShouldClose() override;

    game_engine::GameSettings getSettings() override;

private:
    game_engine::core::Engine& m_engine;

    std::shared_ptr<game_engine::core::Shader> m_shader;
    std::shared_ptr<game_engine::core::Mesh> m_mesh;

    std::size_t m_updatesCount = 0;
    std::size_t m_framesCount  = 0;
};
