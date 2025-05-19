#pragma once

#include <engine/event_system.hpp>
#include <engine/game.hpp>

class Game final : public game_engine::IGame
{
public:
    Game();
    ~Game() override;

    // game_engine::Game
    bool init(std::shared_ptr<game_engine::IEngine> engine) noexcept override;
    void shutdown() noexcept override;

    void onUpdate(std::chrono::nanoseconds elapsedTime) override;
    void onDraw() override;
    bool onShouldClose() override;
    game_engine::GameSettings getSettings() override;

private:
    void subscribeForEvents();
    void unsubscribeFromEvents();

    std::shared_ptr<game_engine::IEngine> m_engine;

    std::vector<game_engine::EventSystem::SubscriptionPtr> m_subscriptions;

    std::shared_ptr<game_engine::graphics::IShader> m_shader;
    std::shared_ptr<game_engine::graphics::IMesh> m_mesh;

    std::size_t m_updatesCount = 0;
    std::size_t m_framesCount  = 0;
};
