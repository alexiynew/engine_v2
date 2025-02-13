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

    // Method to handle window events (e.g., window close)
    virtual void on_window_event(const std::string& event) = 0;
};

class Backend
{
public:
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

    // Subscribe to backend events
    void set_event_handler(std::weak_ptr<BackendEventHandler> handler);

protected:
    // Notify handler about a keyboard input event
    void notify_keyboard_input_event(const KeyboardInputEvent& event);

    // Notify handler about a window event
    void notify_window_event(const std::string& event);

private:
    std::weak_ptr<BackendEventHandler> m_event_handler;
};

std::shared_ptr<Backend> create_backend_instance();

} // namespace game_engine::backend
