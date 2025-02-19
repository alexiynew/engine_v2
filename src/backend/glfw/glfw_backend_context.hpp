#pragma once

#define GLFW_INCLUDE_NONE
#include <unordered_map>

#include <GLFW/glfw3.h>

namespace game_engine::backend
{

class GLFWBackend;

class GLFWBackendContext
{
public:
    static void registerBackend(GLFWwindow* window, GLFWBackend* backend);

    static GLFWBackend* getBackend(GLFWwindow* window);
    static GLFWwindow* getWindow(GLFWBackend* backend);

private:
    using BackendMap = std::unordered_map<GLFWwindow*, GLFWBackend*>;
    using WindowMap  = std::unordered_map<GLFWBackend*, GLFWwindow*>;
    BackendMap m_backendMap;
    WindowMap m_windowMap;

    static GLFWBackendContext& getInstance();

    static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void onWindowResize(GLFWwindow* window, int width, int height);
    static void onWindowMove(GLFWwindow* window, int xpos, int ypos);
    static void onWindowClose(GLFWwindow* window);
    static void onWindowFocus(GLFWwindow* window, int focused);
    static void onWindowIconify(GLFWwindow* window, int iconified);
    static void onWindowMaximize(GLFWwindow* window, int maximized);
};

} // namespace game_engine::backend
