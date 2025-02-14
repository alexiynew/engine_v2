#pragma once

#include <memory>
#include <string>
#include <vector>

#include <game_engine/keyboard.hpp>

namespace game_engine::backend
{

// Interface for observers
class BackendEventHandler
{
public:
    virtual ~BackendEventHandler() = default;

    // Method to handle keyboard input events (e.g., key presses)
    virtual void on_keyboard_input_event(const KeyboardInputEvent& event) = 0;

    // Method to handle window resize events
    // @param width: The new width of the window, in screen coordinates.
    // @param height: The new height of the window, in screen coordinates.
    virtual void on_window_resize(int width, int height) = 0;

    // Method to handle window move events
    // @param xpos: The new x-coordinate of the upper-left corner of the content area of the window, in screen coordinates.
    // @param ypos: The new y-coordinate of the upper-left corner of the content area of the window, in screen coordinates.
    virtual void on_window_move(int xpos, int ypos) = 0;

    // Method to handle window close events
    // This is called when the user attempts to close the window.
    virtual void on_window_close() = 0;

    // Method to handle window focus events
    // @param focused: True if the window gained input focus, false if it lost focus.
    virtual void on_window_focus(bool focused) = 0;

    // Method to handle window iconify events
    // @param iconified: True if the window was iconified (minimized), false if it was restored.
    virtual void on_window_iconify(bool iconified) = 0;

    // Method to handle window maximize events
    // @param maximized: True if the window was maximized, false if it was restored.
    virtual void on_window_maximize(bool maximized) = 0;
};

class Backend
{
public:
    Backend(BackendEventHandler& handler);

    virtual ~Backend() = default;

    // Initialize the backend
    virtual bool initialize() = 0;

    // Shut down the backend
    virtual void shutdown() = 0;

    // Poll events (input, window, etc.)
    virtual void poll_events() = 0;

    // Begin a new frame (e.g., clear the screen)
    virtual void begin_frame() = 0;

    // End the frame (e.g., swap buffers)
    virtual void end_frame() = 0;

protected:
    BackendEventHandler& m_event_handler;
};

std::unique_ptr<Backend> create_backend_instance(BackendEventHandler& handler);

} // namespace game_engine::backend
