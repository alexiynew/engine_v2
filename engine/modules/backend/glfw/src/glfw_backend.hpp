#pragma once

#include <list>
#include <mutex>

#include <modules/backend/backend.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

class GLFWBackend final : public IBackendModule
{
public:

    GLFWBackend();
    ~GLFWBackend() override;

    // IBackend implementation
    bool Init(const GameSettings& settings) noexcept override;
    void Shutdown() noexcept override;

    void AttachBackendObserver(IBackendObserver& observer) override;
    void DetachBackendObserver(const IBackendObserver& observer) override;

    void PollEvents() const override;

    void MakeContextCurrent() const override;
    void DropCurrentContext() const override;
    void SwapBuffers() const override;

    // Event handling
    void HandleKeyEvent(int key, int scancode, int action, int mods);
    void HandleWindowResize(int width, int height);
    void HandleWindowMove(int xpos, int ypos);
    void HandleWindowClose();
    void HandleWindowFocus(bool focused);
    void HandleWindowIconify(bool iconified);
    void HandleWindowMaximize(bool maximized);

private:

    void ApplySettings(const GameSettings& settings);
    void ApplyDisplayMode(const GameSettings& settings);
    void ApplyAntiAliasing(const GameSettings& settings);

    template <typename TEventType>
    void Notify(const TEventType& event);

    mutable std::mutex m_window_mutex;
    GLFWwindow* m_window = nullptr;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
