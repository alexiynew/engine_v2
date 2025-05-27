#include "glfw_backend.hpp"

#include <glfw_keyboard.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace
{
using game_engine::backend::GLFWBackend;

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleKeyEvent(key, scancode, action, mods);
    }
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowResize(width, height);
    }
}

void OnWindowMove(GLFWwindow* window, int xpos, int ypos)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowMove(xpos, ypos);
    }
}

void OnWindowClose(GLFWwindow* window)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowClose();
    }
}

void OnWindowFocus(GLFWwindow* window, int focused)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowFocus(focused == GLFW_TRUE);
    }
}

void OnWindowIconify(GLFWwindow* window, int iconified)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowIconify(iconified == GLFW_TRUE);
    }
}

void OnWindowMaximize(GLFWwindow* window, int maximized)
{
    if (auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window))) {
        backend->HandleWindowMaximize(maximized == GLFW_TRUE);
    }
}

void SetCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, &OnKeyEvent);
    glfwSetWindowSizeCallback(window, &OnWindowResize);
    glfwSetWindowPosCallback(window, &OnWindowMove);
    glfwSetWindowCloseCallback(window, &OnWindowClose);
    glfwSetWindowFocusCallback(window, &OnWindowFocus);
    glfwSetWindowIconifyCallback(window, &OnWindowIconify);
    glfwSetWindowMaximizeCallback(window, &OnWindowMaximize);
}

void DropCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, nullptr);
    glfwSetWindowSizeCallback(window, nullptr);
    glfwSetWindowPosCallback(window, nullptr);
    glfwSetWindowCloseCallback(window, nullptr);
    glfwSetWindowFocusCallback(window, nullptr);
    glfwSetWindowIconifyCallback(window, nullptr);
}

void LogErrors()
{
    const char* error = nullptr;
    const int code    = glfwGetError(&error);
    if (error != nullptr) {
        LOG_ERROR << "GLFW Error: " << code << " " << error << std::endl;
    }
}

} // namespace

namespace game_engine::backend
{

GLFWBackend::GLFWBackend() = default;

GLFWBackend::~GLFWBackend()
{
    Shutdown();
}

#pragma region Backend

bool GLFWBackend::Init(const GameSettings& settings) noexcept
{
    std::lock_guard lock(m_window_mutex);

    if (m_window != nullptr) {
        LOG_ERROR << "Already initialized";
        return false;
    }

    if (glfwInit() == GLFW_FALSE) {
        LogErrors();
        return false;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (settings.display_mode == DisplayMode::BorderlessFullscreen) {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    ApplyAntiAliasing(settings);

    m_window = glfwCreateWindow(settings.resolution_width,
                                settings.resolution_height,
                                settings.window_title.c_str(),
                                nullptr,
                                nullptr);
    if (!m_window) {
        LogErrors();
        glfwTerminate();
        return false;
    }

    ApplySettings(settings);

    glfwSetWindowUserPointer(m_window, this);

    SetCallbacks(m_window);

    return true;
}

void GLFWBackend::Shutdown() noexcept
{
    std::lock_guard lock(m_window_mutex);
    if (m_window) {
        glfwSetWindowUserPointer(m_window, nullptr);
        DropCallbacks(m_window);
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void GLFWBackend::PollEvents()
{
    glfwPollEvents();
}

void GLFWBackend::AttachBackendObserver(IBackendObserver& observer)
{
    m_observers.push_front(observer);
}

void GLFWBackend::DetachBackendObserver(const IBackendObserver& observer)
{
    m_observers.remove_if([&observer](const RefObserver& obj) { return &obj.get() == &observer; });
}

std::shared_ptr<const IRenderContext> GLFWBackend::GetRenderContext() const
{
    return shared_from_this();
}

#pragma endregion

#pragma region RenderContext

void GLFWBackend::MakeCurrent() const
{
    std::lock_guard lock(m_window_mutex);
    glfwMakeContextCurrent(m_window);
}

void GLFWBackend::DropCurrent() const
{
    std::lock_guard lock(m_window_mutex);
    glfwMakeContextCurrent(nullptr);
}

void GLFWBackend::SwapBuffers() const
{
    std::lock_guard lock(m_window_mutex);
    glfwSwapBuffers(m_window);
}

#pragma endregion

#pragma region Event handling

void GLFWBackend::HandleKeyEvent(int key, int scancode, int action, int mods)
{
    KeyboardInputEvent event;
    event.key       = ConvertGlfwKey(key);
    event.action    = ConvertGlfwAction(action);
    event.modifiers = ConvertGlfwModifiers(mods);

    Notify(event);
}

void GLFWBackend::HandleWindowResize(int width, int height)
{
    Notify(WindowResizeEvent{width, height});
}

void GLFWBackend::HandleWindowMove(int xpos, int ypos)
{
    Notify(WindowMoveEvent{xpos, ypos});
}

void GLFWBackend::HandleWindowClose()
{
    Notify(WindowCloseEvent{});
}

void GLFWBackend::HandleWindowFocus(bool focused)
{
    Notify(WindowFocusEvent{focused});
}

void GLFWBackend::HandleWindowIconify(bool iconified)
{
    Notify(WindowIconifyEvent{iconified});
}

void GLFWBackend::HandleWindowMaximize(bool maximized)
{
    Notify(WindowMaximizeEvent{maximized});
}

#pragma endregion

#pragma region GLFWBackend private

void GLFWBackend::ApplySettings(const GameSettings& settings)
{
    ApplyDisplayMode(settings);

    glfwSetWindowTitle(m_window, settings.window_title.c_str());

    glfwSwapInterval(settings.v_sync ? 1 : 0);
}

void GLFWBackend::ApplyDisplayMode(const GameSettings& settings)
{
    GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    switch (settings.display_mode) {
        case DisplayMode::Fullscreen:
            glfwSetWindowMonitor(m_window,
                                 monitor,
                                 0,
                                 0,
                                 settings.resolution_width,
                                 settings.resolution_height,
                                 mode->refreshRate);
            break;
        case DisplayMode::BorderlessFullscreen:
            glfwSetWindowMonitor(m_window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
            break;
        case DisplayMode::Windowed: {
            const int x_pos = (mode->width - settings.resolution_width) / 2;
            const int y_pos = (mode->height - settings.resolution_height) / 2;

            glfwSetWindowMonitor(m_window,
                                 nullptr,
                                 x_pos,
                                 y_pos,
                                 settings.resolution_width,
                                 settings.resolution_height,
                                 mode->refreshRate);
        } break;
    }
}

void GLFWBackend::ApplyAntiAliasing(const GameSettings& settings)
{
    switch (settings.anti_aliasing) {
        case AntiAliasing::None:   glfwWindowHint(GLFW_SAMPLES, 0); break;
        case AntiAliasing::MSAA2x: glfwWindowHint(GLFW_SAMPLES, 2); break;
        case AntiAliasing::MSAA4x: glfwWindowHint(GLFW_SAMPLES, 4); break;
        case AntiAliasing::MSAA8x: glfwWindowHint(GLFW_SAMPLES, 8); break;
    }
}

template <typename TEventType>
void GLFWBackend::Notify(const TEventType& event)
{
    for (auto observer : m_observers) {
        observer.get().OnEvent(event);
    }
}

#pragma endregion

} // namespace game_engine::backend
