#include "glfw_backend.hpp"

#include <unordered_map>

#include <glfw_keyboard.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace
{
using namespace game_engine::backend;

class GLFWBackendContext
{
public:
    static void register_backend(GLFWwindow* window, GLFWBackend* backend) {
        get_instance().m_backend_map[window] = backend;
        get_instance().m_window_map[backend] = window;

        glfwSetKeyCallback(window, &GLFWBackendContext::on_key_event);
        glfwSetWindowSizeCallback(window, &GLFWBackendContext::on_window_resize);
        glfwSetWindowPosCallback(window, &GLFWBackendContext::on_window_move);
        glfwSetWindowCloseCallback(window, &GLFWBackendContext::on_window_close);
        glfwSetWindowFocusCallback(window, &GLFWBackendContext::on_window_focus);
        glfwSetWindowIconifyCallback(window, &GLFWBackendContext::on_window_iconify);
        glfwSetWindowMaximizeCallback(window, &GLFWBackendContext::on_window_maximize);
    }

    static GLFWBackend* get_backend(GLFWwindow* window) {
        auto it = get_instance().m_backend_map.find(window);
        return it != get_instance().m_backend_map.end() ? it->second : nullptr;
    }

    static GLFWwindow* get_window(GLFWBackend* backend) {
        auto it = get_instance().m_window_map.find(backend);
        return it != get_instance().m_window_map.end() ? it->second : nullptr;
    }

private:
    using BackendMap = std::unordered_map<GLFWwindow*, GLFWBackend*>;
    using WindowMap  = std::unordered_map<GLFWBackend*, GLFWwindow*>;
    BackendMap m_backend_map;
    WindowMap m_window_map;

    static GLFWBackendContext& get_instance() {
        static GLFWBackendContext instance;
        return instance;
    }

    static void on_key_event(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void on_window_resize(GLFWwindow* window, int width, int height);
    static void on_window_move(GLFWwindow* window, int xpos, int ypos);
    static void on_window_close(GLFWwindow* window);
    static void on_window_focus(GLFWwindow* window, int focused);
    static void on_window_iconify(GLFWwindow* window, int iconified);
    static void on_window_maximize(GLFWwindow* window, int maximized);
};

void GLFWBackendContext::on_key_event(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_key_event(key, scancode, action, mods);
    }
}

void GLFWBackendContext::on_window_resize(GLFWwindow* window, int width, int height) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_resize(width, height);
    }
}

void GLFWBackendContext::on_window_move(GLFWwindow* window, int xpos, int ypos) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_move(xpos, ypos);
    }
}

void GLFWBackendContext::on_window_close(GLFWwindow* window) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_close();
    }
}

void GLFWBackendContext::on_window_focus(GLFWwindow* window, int focused) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_focus(focused == GLFW_TRUE);
    }
}

void GLFWBackendContext::on_window_iconify(GLFWwindow* window, int iconified) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_iconify(iconified == GLFW_TRUE);
    }
}

void GLFWBackendContext::on_window_maximize(GLFWwindow* window, int maximized) {
    if (GLFWBackend* backend = GLFWBackendContext::get_backend(window); backend) {
        backend->handle_window_maximize(maximized == GLFW_TRUE);
    }
}

} // namespace

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

    auto window = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    GLFWBackendContext::register_backend(window, this);

    glfwMakeContextCurrent(GLFWBackendContext::get_window(this));

    const int version = gladLoadGL();
    if (version == 0) {
        glfwTerminate();
        return false;
    }

    return true;
}

void GLFWBackend::shutdown() {
    glfwDestroyWindow(GLFWBackendContext::get_window(this));
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
    glfwSwapBuffers(GLFWBackendContext::get_window(this));
}

void GLFWBackend::handle_key_event(int key, int scancode, int action, int mods) {
    KeyboardInputEvent event;
    event.key       = convert_glfw_key(key);
    event.action    = convert_glfw_action(action);
    event.modifiers = convert_glfw_modifiers(mods);

    m_event_handler.on_keyboard_input_event(event);
}

void GLFWBackend::handle_window_resize(int width, int height) {
    m_event_handler.on_window_resize(width, height);
}

void GLFWBackend::handle_window_move(int xpos, int ypos) {
    m_event_handler.on_window_move(xpos, ypos);
}

void GLFWBackend::handle_window_close() {
    m_event_handler.on_window_close();
}

void GLFWBackend::handle_window_focus(bool focused) {
    m_event_handler.on_window_focus(focused);
}

void GLFWBackend::handle_window_iconify(bool iconified) {
    m_event_handler.on_window_iconify(iconified);
}

void GLFWBackend::handle_window_maximize(bool maximized) {
    m_event_handler.on_window_maximize(maximized);
}

} // namespace game_engine::backend
