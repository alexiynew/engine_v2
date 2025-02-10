#include "glfw_backend.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace game_engine::backend
{

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
    glfwMakeContextCurrent(m_impl->window);
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
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWBackend::end_frame() {
    glfwSwapBuffers(m_impl->window);
}

} // namespace game_engine::backend
