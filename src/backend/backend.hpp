#pragma once

#include <list>
#include <memory>
#include <string>

#include <game_engine/core/engine.hpp>
#include <game_engine/system_events.hpp>

namespace game_engine::backend
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

class Backend
{
public:
    using RefObserver = std::reference_wrapper<BackendObserver>;

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

    void attachBackendObserver(BackendObserver& observer);
    void detachBackendObserver(BackendObserver& observer);

protected:
    std::list<RefObserver> m_observers;

    template <typename EventType>
    void notify(const EventType& event)
    {
        for (auto observer : m_observers) {
            observer.get().onEvent(event);
        }
    }
};

std::shared_ptr<Backend> createBackendInstance();

} // namespace game_engine::backend
