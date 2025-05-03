#include "glfw_backend.hpp"

#include <glfw/glfw_keyboard.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace
{
using game_engine::backend::GLFWBackend;

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleKeyEvent(key, scancode, action, mods);
    }
}

void onWindowResize(GLFWwindow* window, int width, int height)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowResize(width, height);
    }
}

void onWindowMove(GLFWwindow* window, int xpos, int ypos)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowMove(xpos, ypos);
    }
}

void onWindowClose(GLFWwindow* window)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowClose();
    }
}

void onWindowFocus(GLFWwindow* window, int focused)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowFocus(focused == GLFW_TRUE);
    }
}

void onWindowIconify(GLFWwindow* window, int iconified)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowIconify(iconified == GLFW_TRUE);
    }
}

void onWindowMaximize(GLFWwindow* window, int maximized)
{
    if (GLFWBackend* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->handleWindowMaximize(maximized == GLFW_TRUE);
    }
}

void setCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, &onKeyEvent);
    glfwSetWindowSizeCallback(window, &onWindowResize);
    glfwSetWindowPosCallback(window, &onWindowMove);
    glfwSetWindowCloseCallback(window, &onWindowClose);
    glfwSetWindowFocusCallback(window, &onWindowFocus);
    glfwSetWindowIconifyCallback(window, &onWindowIconify);
    glfwSetWindowMaximizeCallback(window, &onWindowMaximize);
}

void dropCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, nullptr);
    glfwSetWindowSizeCallback(window, nullptr);
    glfwSetWindowPosCallback(window, nullptr);
    glfwSetWindowCloseCallback(window, nullptr);
    glfwSetWindowFocusCallback(window, nullptr);
    glfwSetWindowIconifyCallback(window, nullptr);
}

void logErrors()
{
    const char* error = nullptr;
    const int code    = glfwGetError(&error);
    if (error) {
        LOG_ERROR << "GLFW Error: " << code << " " << error << std::endl;
    }
}

} // namespace

namespace game_engine::backend
{

GLFWBackend::GLFWBackend()
{}

GLFWBackend::~GLFWBackend()
{
    shutdown();
}

#pragma region Backend

bool GLFWBackend::initialize(const GameSettings& settings)
{
    std::lock_guard lock(m_windowMutex);

    if (m_window) {
        LOG_ERROR << "Already initialized";
        return false;
    }

    if (!glfwInit()) {
        logErrors();
        return false;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (settings.displayMode == DisplayMode::BorderlessFullscreen) {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    applyAntiAliasing(settings);

    m_window = glfwCreateWindow(settings.resolutionWidth,
                                settings.resolutionHeight,
                                settings.windowTitle.c_str(),
                                nullptr,
                                nullptr);
    if (!m_window) {
        logErrors();
        glfwTerminate();
        return false;
    }

    applySettings(settings);

    glfwSetWindowUserPointer(m_window, this);

    setCallbacks(m_window);

    return true;
}

void GLFWBackend::shutdown()
{
    std::lock_guard lock(m_windowMutex);
    if (m_window) {
        glfwSetWindowUserPointer(m_window, nullptr);
        dropCallbacks(m_window);
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void GLFWBackend::pollEvents()
{
    glfwPollEvents();
}

std::shared_ptr<RenderContext> GLFWBackend::getRenderContext()
{
    return shared_from_this();
}

#pragma endregion

#pragma region renderer::RenderContext

void GLFWBackend::makeCurrent()
{
    std::lock_guard lock(m_windowMutex);
    glfwMakeContextCurrent(m_window);
}

void GLFWBackend::dropCurrent()
{
    std::lock_guard lock(m_windowMutex);
    glfwMakeContextCurrent(nullptr);
}

void GLFWBackend::swapBuffers()
{
    std::lock_guard lock(m_windowMutex);
    glfwSwapBuffers(m_window);
}

#pragma endregion

#pragma region Event handling

void GLFWBackend::handleKeyEvent(int key, int scancode, int action, int mods)
{
    KeyboardInputEvent event;
    event.key       = convertGLFWKey(key);
    event.action    = convertGLFWAction(action);
    event.modifiers = convertGLFWModifiers(mods);

    notify(event);
}

void GLFWBackend::handleWindowResize(int width, int height)
{
    notify(WindowResizeEvent{width, height});
}

void GLFWBackend::handleWindowMove(int xpos, int ypos)
{
    notify(WindowMoveEvent{xpos, ypos});
}

void GLFWBackend::handleWindowClose()
{
    notify(WindowCloseEvent{});
}

void GLFWBackend::handleWindowFocus(bool focused)
{
    notify(WindowFocusEvent{focused});
}

void GLFWBackend::handleWindowIconify(bool iconified)
{
    notify(WindowIconifyEvent{iconified});
}

void GLFWBackend::handleWindowMaximize(bool maximized)
{
    notify(WindowMaximizeEvent{maximized});
}

#pragma endregion

#pragma region GLFWBackend private

void GLFWBackend::applySettings(const GameSettings& settings)
{
    applyDisplayMode(settings);

    glfwSetWindowTitle(m_window, settings.windowTitle.c_str());

    glfwSwapInterval(settings.vSync ? 1 : 0);
}

void GLFWBackend::applyDisplayMode(const GameSettings& settings)
{
    GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    switch (settings.displayMode) {
        case DisplayMode::Fullscreen:
            glfwSetWindowMonitor(m_window,
                                 monitor,
                                 0,
                                 0,
                                 settings.resolutionWidth,
                                 settings.resolutionHeight,
                                 mode->refreshRate);
            break;
        case DisplayMode::BorderlessFullscreen:
            glfwSetWindowMonitor(m_window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
            break;
        case DisplayMode::Windowed: {
            const int xPos = (mode->width - settings.resolutionWidth) / 2;
            const int yPos = (mode->height - settings.resolutionHeight) / 2;

            glfwSetWindowMonitor(m_window,
                                 nullptr,
                                 xPos,
                                 yPos,
                                 settings.resolutionWidth,
                                 settings.resolutionHeight,
                                 mode->refreshRate);
        } break;
    }
}

void GLFWBackend::applyAntiAliasing(const GameSettings& settings)
{
    switch (settings.antiAliasing) {
        case AntiAliasing::None:   glfwWindowHint(GLFW_SAMPLES, 0); break;
        case AntiAliasing::MSAA2x: glfwWindowHint(GLFW_SAMPLES, 2); break;
        case AntiAliasing::MSAA4x: glfwWindowHint(GLFW_SAMPLES, 4); break;
        case AntiAliasing::MSAA8x: glfwWindowHint(GLFW_SAMPLES, 8); break;
    }
}

#pragma endregion

} // namespace game_engine::backend
