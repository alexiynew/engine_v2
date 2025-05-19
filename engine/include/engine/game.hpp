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

    virtual bool init(std::shared_ptr<game_engine::IEngine> engine) noexcept = 0;
    virtual void shutdown() noexcept                                         = 0;

    /// @brief Called every frame to update the game state.
    /// @param elapsedTime The time elapsed since the last update.
    virtual void onUpdate(std::chrono::nanoseconds elapsedTime) = 0;

    /// @brief Called every frame to render the game.
    virtual void onDraw() = 0;

    /// @brief Checks if the game should close.
    /// @return True if the game should close, false otherwise.
    virtual bool onShouldClose() = 0;

    /// @brief Get current game settings.
    /// @return Current game settings.
    virtual GameSettings getSettings() = 0;
};

} // namespace game_engine
