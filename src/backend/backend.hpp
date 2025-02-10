#pragma once

namespace game_engine::backend
{

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
};

} // namespace game_engine::backend
