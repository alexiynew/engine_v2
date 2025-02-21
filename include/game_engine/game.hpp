#pragma once

#include <chrono>
#include <memory>

#include <game_engine/core/engine.hpp>
#include <game_engine/keyboard.hpp>
#include <game_engine/system_events.hpp>

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

    /// @brief Called when a keyboard input event occurs.
    /// @param event The keyboard input event.
    virtual void onKeyboardInputEvent(const KeyboardInputEvent& event) = 0;

    /// @brief Checks if the game should close.
    /// @return True if the game should close, false otherwise.
    virtual bool onShouldClose() = 0;
};

/// @brief Creates a new instance of the game.
/// @param engine The engine instance to associate with the game.
/// @return A unique pointer to the created game instance.
std::unique_ptr<Game> createGameInstance(core::Engine& engine);

} // namespace game_engine
