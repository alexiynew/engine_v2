#pragma once

#include <memory>
#include <string>
#include <vector>

#include <game_engine/core/engine.hpp>
#include <game_engine/keyboard.hpp>

namespace game_engine::backend
{

/// @brief Interface for observers
class BackendEventHandler
{
public:
    virtual ~BackendEventHandler() = default;

    /// @brief Method to handle keyboard input events (e.g., key presses)
    virtual void onKeyboardInputEvent(const KeyboardInputEvent& event) = 0;

    /// @brief Method to handle window resize events
    /// @param width: The new width of the window, in screen coordinates.
    /// @param height: The new height of the window, in screen coordinates.
    virtual void onWindowResize(int width, int height) = 0;

    /// @brief Method to handle window move events
    /// @param xpos: The new x-coordinate of the upper-left corner of the content area of the window, in screen coordinates.
    /// @param ypos: The new y-coordinate of the upper-left corner of the content area of the window, in screen coordinates.
    virtual void onWindowMove(int xpos, int ypos) = 0;

    /// @brief Method to handle window close events
    /// This is called when the user attempts to close the window.
    virtual void onWindowClose() = 0;

    /// @brief Method to handle window focus events
    /// @param focused: True if the window gained input focus, false if it lost focus.
    virtual void onWindowFocus(bool focused) = 0;

    /// @brief Method to handle window iconify events
    /// @param iconified: True if the window was iconified (minimized), false if it was restored.
    virtual void onWindowIconify(bool iconified) = 0;

    /// @brief Method to handle window maximize events
    /// @param maximized: True if the window was maximized, false if it was restored.
    virtual void onWindowMaximize(bool maximized) = 0;
};

class Backend
{
public:
    Backend(BackendEventHandler& handler);

    virtual ~Backend() = default;

    /// @brief Initialize the backend
    virtual bool initialize() = 0;

    /// @brief Shut down the backend
    virtual void shutdown() = 0;

    /// @brief Poll events (input, window, etc.)
    virtual void pollEvents() = 0;

    /// @brief Begin a new frame (e.g., clear the screen)
    virtual void beginFrame() = 0;

    /// @brief End the frame (e.g., swap buffers)
    virtual void endFrame() = 0;

    /// @brief Loads mesh to backend.
    /// @param mesh The model mesh to load.
    /// @return The ID of the loaded mesh.
    virtual core::MeshId loadMesh(const core::Mesh& mesh) = 0;

    /// @brief Renders a model by its ID.
    /// @param meshId The ID of the model to render.
    virtual void renderMesh(core::MeshId meshId) = 0;

protected:
    BackendEventHandler& m_eventHandler;
};

std::unique_ptr<Backend> createBackendInstance(BackendEventHandler& handler);

} // namespace game_engine::backend
