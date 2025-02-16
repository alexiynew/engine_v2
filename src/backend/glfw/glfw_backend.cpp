#include "glfw_backend.hpp"

#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <glfw_keyboard.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{
using namespace game_engine::backend;

const char* c_vertex_shader_source = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

const char* c_fragment_shader_source = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    GLFWBackendContext::register_backend(window, this);

    glfwMakeContextCurrent(GLFWBackendContext::get_window(this));

    if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
        glfwTerminate();
        return false;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_shader.link(c_vertex_shader_source, c_fragment_shader_source);

    return true;
}

void GLFWBackend::shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    m_shader.clear();

    glfwDestroyWindow(GLFWBackendContext::get_window(this));
    glfwTerminate();
}

void GLFWBackend::poll_events() {
    glfwPollEvents();
}

void GLFWBackend::begin_frame() {
    glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.use();

    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model      = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    auto u_model      = m_shader.get_uniform_location("model");
    auto u_view       = m_shader.get_uniform_location("view");
    auto u_projection = m_shader.get_uniform_location("projection");

    m_shader.set_uniform(u_model, model);
    m_shader.set_uniform(u_view, view);
    m_shader.set_uniform(u_projection, projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
