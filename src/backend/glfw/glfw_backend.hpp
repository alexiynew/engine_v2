#pragma once

#include <mutex>

#include <backend/backend.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

class GLFWBackend final
    : public Backend
    , public renderer::RendererContext
    , public std::enable_shared_from_this<GLFWBackend>
{
public:
    GLFWBackend();
    ~GLFWBackend() override;

    // Backend
    bool initialize(const GameSettings& settings) override;
    void shutdown() override;
    void pollEvents() override;

    std::shared_ptr<renderer::RendererContext> getRendererContext() override;

    // renderer::RendererContext
    void makeCurrent() override;
    void dropCurrent() override;
    void swapBuffers() override;

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

    std::mutex m_windowMutex;
    GLFWwindow* m_window = nullptr;
};

} // namespace game_engine::backend
