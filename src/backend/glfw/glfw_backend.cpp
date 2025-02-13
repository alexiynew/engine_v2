#include "glfw_backend.hpp"

#include <glfw_keyboard.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace
{
void key_callback_handler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    using namespace game_engine;
    using namespace game_engine::backend;

    GLFWBackend* backend = reinterpret_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));

    if (backend == nullptr) {
        return;
    }

    backend->key_callback(key, scancode, action, mods);
}
} // namespace

namespace game_engine::backend
{

std::shared_ptr<Backend> create_backend_instance() {
    return std::make_shared<GLFWBackend>();
}

struct GLFWBackend::Impl
{
    GLFWwindow* window = nullptr;
};

GLFWBackend::GLFWBackend()
    : m_impl(std::make_unique<Impl>()) {
}

GLFWBackend::~GLFWBackend() = default;

bool GLFWBackend::initialize() {
    if (!glfwInit()) {
        return false;
    }

    m_impl->window = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);
    if (!m_impl->window) {
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(m_impl->window, this);

    glfwMakeContextCurrent(m_impl->window);

    glfwSetKeyCallback(m_impl->window, key_callback_handler);

    const int version = gladLoadGL();
    if (version == 0) {
        glfwTerminate();
        return false;
    }

    return true;
}

void GLFWBackend::shutdown() {
    glfwDestroyWindow(m_impl->window);
    glfwTerminate();
}

void GLFWBackend::poll_events() {
    glfwPollEvents();
}

void GLFWBackend::begin_frame() {
    glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWBackend::end_frame() {
    glfwSwapBuffers(m_impl->window);
}

void GLFWBackend::key_callback(int key, int scancode, int action, int mods) {
    KeyboardInputEvent event;
    event.key       = convert_glfw_key(key);
    event.action    = convert_glfw_action(action);
    event.modifiers = convert_glfw_modifiers(mods);

    notify_keyboard_input_event(event);
}

} // namespace game_engine::backend
