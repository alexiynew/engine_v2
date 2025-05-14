#pragma once

#include <mutex>

#include <modules/backend/backend.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

class GLFWBackend final
    : public Backend
    , public RenderContext
    , public std::enable_shared_from_this<GLFWBackend>
{
public:
    GLFWBackend();
    ~GLFWBackend() override;

    // Backend
    bool init(const GameSettings& settings) noexcept override;
    void shutdown() noexcept override;

    void pollEvents() override;

    std::shared_ptr<const RenderContext> getRenderContext() const override;

    void attachBackendObserver(BackendObserver& observer) override;
    void detachBackendObserver(const BackendObserver& observer) override;

    // RenderContext
    void makeCurrent() const override;
    void dropCurrent() const override;
    void swapBuffers() const override;

    // Event handling
    void handleKeyEvent(int key, int scancode, int action, int mods);
    void handleWindowResize(int width, int height);
    void handleWindowMove(int xpos, int ypos);
    void handleWindowClose();
    void handleWindowFocus(bool focused);
    void handleWindowIconify(bool iconified);
    void handleWindowMaximize(bool maximized);

private:

    void applySettings(const GameSettings& settings);
    void applyDisplayMode(const GameSettings& settings);
    void applyAntiAliasing(const GameSettings& settings);

    template <typename EventType>
    void notify(const EventType& event);

    mutable std::mutex m_windowMutex;
    GLFWwindow* m_window = nullptr;

    std::list<RefObserver> m_observers;
};

} // namespace game_engine::backend
