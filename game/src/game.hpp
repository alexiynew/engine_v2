#pragma once

#include <engine/events/event_system.hpp>
#include <engine/game.hpp>

class Game final : public game_engine::IGame
{
public:

    Game();
    ~Game() override;

    // IGame implementation
    bool Init(std::shared_ptr<game_engine::IEngine> engine) noexcept override;
    void Shutdown() noexcept override;

    void OnUpdate(std::chrono::nanoseconds elapsed_time) override;
    void OnDraw() override;
    bool OnShouldClose() override;
    game_engine::GameSettings GetSettings() override;

private:

    void SubscribeForEvents();
    void UnsubscribeFromEvents();

    std::shared_ptr<game_engine::IEngine> m_engine;

    std::vector<game_engine::IEventSystem::SubscriptionPtr> m_subscriptions;

    std::shared_ptr<game_engine::IShader> m_shader;
    std::shared_ptr<game_engine::IMesh> m_mesh;

    std::size_t m_updates_count = 0;
    std::size_t m_frames_count  = 0;
};
