#pragma once

#include <list>
#include <memory>
#include <string>

#include <game_engine/game_settings.hpp>
#include <game_engine/system_events.hpp>

#include <modules/render_context.hpp>

namespace game_engine
{

/// @brief Interface for observers
class BackendObserver

{
public:
    virtual ~BackendObserver() = default;

    /// @brief Method to handle keyboard input events (e.g., key presses)
    virtual void onEvent(const KeyboardInputEvent& event) = 0;

    /// @brief Method to handle window resize events
    virtual void onEvent(const WindowResizeEvent& event) = 0;

    /// @brief Method to handle window move events
    virtual void onEvent(const WindowMoveEvent& event) = 0;

    /// @brief Method to handle window close events
    /// This is called when the user attempts to close the window.
    virtual void onEvent(const WindowCloseEvent& event) = 0;

    /// @brief Method to handle window focus events
    virtual void onEvent(const WindowFocusEvent& event) = 0;

    /// @brief Method to handle window iconify events
    virtual void onEvent(const WindowIconifyEvent& event) = 0;

    /// @brief Method to handle window maximize events
    virtual void onEvent(const WindowMaximizeEvent& event) = 0;
};

} // namespace game_engine
