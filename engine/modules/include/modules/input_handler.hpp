#pragma once

#include <list>
#include <memory>
#include <string>

#include <engine/game_settings.hpp>
#include <engine/window_events.hpp>

namespace game_engine
{

/// @brief Interface for observers
class IBackendObserver

{
public:
    virtual ~IBackendObserver() = default;

    /// @brief Method to handle keyboard input events (e.g., key presses)
    virtual void OnEvent(const KeyboardInputEvent& event) = 0;

    /// @brief Method to handle window resize events
    virtual void OnEvent(const WindowResizeEvent& event) = 0;

    /// @brief Method to handle window move events
    virtual void OnEvent(const WindowMoveEvent& event) = 0;

    /// @brief Method to handle window close events
    /// This is called when the user attempts to close the window.
    virtual void OnEvent(const WindowCloseEvent& event) = 0;

    /// @brief Method to handle window focus events
    virtual void OnEvent(const WindowFocusEvent& event) = 0;

    /// @brief Method to handle window iconify events
    virtual void OnEvent(const WindowIconifyEvent& event) = 0;

    /// @brief Method to handle window maximize events
    virtual void OnEvent(const WindowMaximizeEvent& event) = 0;
};

} // namespace game_engine
