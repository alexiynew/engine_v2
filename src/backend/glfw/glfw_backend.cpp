#include "glfw_backend.hpp"

#include <glad/glad.h>
#include <glfw_keyboard.hpp>

namespace game_engine::backend
{

std::unique_ptr<Backend> create_backend_instance(BackendEventHandler& handler) {
    return std::make_unique<GLFWBackend>(handler);
}

GLFWBackend::GLFWBackend(BackendEventHandler& handler)
    : Backend(handler) {
}

GLFWBackend::~GLFWBackend() = default;

bool GLFWBackend::initialize() {
    if (!glfwInit()) {
        return false;
    }

    m_window = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwMakeContextCurrent(m_window);

    glfwSetKeyCallback(m_window, &GLFWBackend::key_callback);
    glfwSetWindowSizeCallback(m_window, &GLFWBackend::window_size_callback);
    glfwSetWindowPosCallback(m_window, &GLFWBackend::window_pos_callback);
    glfwSetWindowCloseCallback(m_window, &GLFWBackend::window_close_callback);
    glfwSetWindowFocusCallback(m_window, &GLFWBackend::window_focus_callback);
    glfwSetWindowIconifyCallback(m_window, &GLFWBackend::window_iconify_callback);
    glfwSetWindowMaximizeCallback(m_window, &GLFWBackend::window_maximize_callback);

    const int version = gladLoadGL();
    if (version == 0) {
        glfwTerminate();
        return false;
    }

    return true;
}

void GLFWBackend::shutdown() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GLFWBackend::poll_events() {
    glfwPollEvents();
}

void GLFWBackend::begin_frame() {
    glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWBackend::end_frame() {
    glfwSwapBuffers(m_window);
}

void GLFWBackend::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        KeyboardInputEvent event;
        event.key       = convert_glfw_key(key);
        event.action    = convert_glfw_action(action);
        event.modifiers = convert_glfw_modifiers(mods);

        backend->m_event_handler.on_keyboard_input_event(event);
    }
}

void GLFWBackend::window_size_callback(GLFWwindow* window, int width, int height) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_resize(width, height);
    }
}

void GLFWBackend::window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_move(xpos, ypos);
    }
}

void GLFWBackend::window_close_callback(GLFWwindow* window) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_close();
    }
}

void GLFWBackend::window_focus_callback(GLFWwindow* window, int focused) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_focus(focused == GLFW_TRUE);
    }
}

void GLFWBackend::window_iconify_callback(GLFWwindow* window, int iconified) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_iconify(iconified == GLFW_TRUE);
    }
}

void GLFWBackend::window_maximize_callback(GLFWwindow* window, int maximized) {
    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
    if (backend) {
        backend->m_event_handler.on_window_maximize(maximized == GLFW_TRUE);
    }
}

} // namespace game_engine::backend
