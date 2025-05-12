#pragma once

#include <engine/keyboard.hpp>

namespace game_engine
{

struct KeyboardInputEvent
{
    KeyCode key           = KeyCode::Unknown;  ///< The key that was pressed or released.
    KeyAction action      = KeyAction::None;   ///< The action performed on the key.
    KeyModifier modifiers = KeyModifier::None; ///< The modifiers active during the event.
};

struct WindowResizeEvent
{
    int width  = 0; ///< The new width of the window, in screen coordinates.
    int height = 0; ///< The new height of the window, in screen coordinates.
};

struct WindowMoveEvent
{
    int
    xpos = 0; ///< The new x-position of the upper-left corner of the content area of the window, in screen coordinates.
    int
    ypos = 0; ///< The new y-position of the upper-left corner of the content area of the window, in screen coordinates.
};

struct WindowCloseEvent
{};

struct WindowFocusEvent
{
    bool focused = false; ///< True if the window gained input focus, false if it lost focus.
};

struct WindowIconifyEvent
{
    bool iconified = false; ///< True if the window was iconified (minimized), false if it was restored.
};

struct WindowMaximizeEvent
{
    bool maximized = false; ///< True if the window was maximized, false if it was restored.
};

} // namespace game_engine
