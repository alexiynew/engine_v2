#pragma once

#include <chrono>
#include <memory>

#include <engine/engine.hpp>
#include <engine/game_settings.hpp>

namespace game_engine
{

class IGame
{
public:

    static std::shared_ptr<IGame> Create();

    virtual ~IGame() = default;

    virtual bool Init(std::shared_ptr<game_engine::IEngine> engine) noexcept = 0;
    virtual void Shutdown() noexcept                                         = 0;

    /// @brief Called every frame to update the game state.
    /// @param elapsed_time The time elapsed since the last update.
    virtual void OnUpdate(std::chrono::nanoseconds elapsed_time) = 0;

    /// @brief Called every frame to render the game.
    virtual void OnDraw() = 0;

    /// @brief Checks if the game should close.
    /// @return True if the game should close, false otherwise.
    virtual bool OnShouldClose() = 0;

    /// @brief Get current game settings.
    /// @return Current game settings.
    virtual GameSettings GetSettings() = 0;
};

} // namespace game_engine
