#include "glfw_backend_context.hpp"

#include <glfw/glfw_backend.hpp>

namespace game_engine::backend
{

GLFWBackendContext& GLFWBackendContext::getInstance()
{
    static GLFWBackendContext instance;
    return instance;
}

void GLFWBackendContext::registerBackend(GLFWwindow* window, GLFWBackend* backend)
{
    getInstance().m_backendMap[window] = backend;
    getInstance().m_windowMap[backend] = window;

    glfwSetKeyCallback(window, &GLFWBackendContext::onKeyEvent);
    glfwSetWindowSizeCallback(window, &GLFWBackendContext::onWindowResize);
    glfwSetWindowPosCallback(window, &GLFWBackendContext::onWindowMove);
    glfwSetWindowCloseCallback(window, &GLFWBackendContext::onWindowClose);
    glfwSetWindowFocusCallback(window, &GLFWBackendContext::onWindowFocus);
    glfwSetWindowIconifyCallback(window, &GLFWBackendContext::onWindowIconify);
    glfwSetWindowMaximizeCallback(window, &GLFWBackendContext::onWindowMaximize);
}

GLFWBackend* GLFWBackendContext::getBackend(GLFWwindow* window)
{
    auto it = getInstance().m_backendMap.find(window);
    return it != getInstance().m_backendMap.end() ? it->second : nullptr;
}

GLFWwindow* GLFWBackendContext::getWindow(GLFWBackend* backend)
{
    auto it = getInstance().m_windowMap.find(backend);
    return it != getInstance().m_windowMap.end() ? it->second : nullptr;
}

void GLFWBackendContext::onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleKeyEvent(key, scancode, action, mods);
    }
}

void GLFWBackendContext::onWindowResize(GLFWwindow* window, int width, int height)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowResize(width, height);
    }
}

void GLFWBackendContext::onWindowMove(GLFWwindow* window, int xpos, int ypos)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowMove(xpos, ypos);
    }
}

void GLFWBackendContext::onWindowClose(GLFWwindow* window)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowClose();
    }
}

void GLFWBackendContext::onWindowFocus(GLFWwindow* window, int focused)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowFocus(focused == GLFW_TRUE);
    }
}

void GLFWBackendContext::onWindowIconify(GLFWwindow* window, int iconified)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowIconify(iconified == GLFW_TRUE);
    }
}

void GLFWBackendContext::onWindowMaximize(GLFWwindow* window, int maximized)
{
    if (GLFWBackend* backend = GLFWBackendContext::getBackend(window); backend) {
        backend->handleWindowMaximize(maximized == GLFW_TRUE);
    }
}

} // namespace game_engine::backend
