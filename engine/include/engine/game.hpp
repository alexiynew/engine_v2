#pragma once

#include <chrono>
#include <memory>

#include <engine/engine.hpp>
#include <engine/game_settings.hpp>

namespace game_engine
{

class Game
{
public:
    virtual ~Game() = default;

    /// @brief Called when the game is initialized.
    virtual void onInitialize() = 0;

    /// @brief Called every frame to update the game state.
    /// @param elapsedTime The time elapsed since the last update.
    virtual void onUpdate(std::chrono::nanoseconds elapsedTime) = 0;

    /// @brief Called every frame to render the game.
    virtual void onDraw() = 0;

    /// @brief Called when the game is shutting down.
    virtual void onShutdown() = 0;

    /// @brief Checks if the game should close.
    /// @return True if the game should close, false otherwise.
    virtual bool onShouldClose() = 0;

    /// @brief Get current game settings.
    /// @return Current game settings.
    virtual GameSettings getSettings() = 0;
};

std::shared_ptr<Game> CreateGameInstance(Engine& engine);

} // namespace game_engine
