#include "glfw_backend.hpp"

#include <unordered_map>

#include <game_engine/common_types.hpp>

#include <glfw/glfw_backend_context.hpp>
#include <glfw/glfw_keyboard.hpp>
#include <glfw/opengl_shader.hpp>
#include <glfw/opengl_utils.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{
using namespace game_engine::backend;

const char* g_vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aUV;
        layout(location = 3) in vec3 aColor;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec4 color;
        void main() {
            color = vec4(aColor, 1.0);
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

const char* g_fragmentShaderSource = R"(
        #version 330 core
        in vec4 color;

        out vec4 FragColor;

        void main() {
            FragColor = color;
            //FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

} // namespace

namespace game_engine::backend
{

std::shared_ptr<Backend> createBackendInstance()
{
    return std::make_shared<GLFWBackend>();
}

GLFWBackend::GLFWBackend()
{
    m_shader = std::make_unique<OpenGLShader>();
}

GLFWBackend::~GLFWBackend() = default;

bool GLFWBackend::initialize()
{
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // TODO: Move window parameters to config
    auto window = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    GLFWBackendContext::registerBackend(window, this);

    glfwMakeContextCurrent(GLFWBackendContext::getWindow(this));

    if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
        glfwTerminate();
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    m_shader->link(g_vertexShaderSource, g_fragmentShaderSource);

    return true;
}

void GLFWBackend::shutdown()
{
    for (auto& [_, m] : m_loadedMeshes) {
        glDeleteVertexArrays(1, &m.VAO);
        glDeleteBuffers(1, &m.VBO);
        glDeleteBuffers(1, &m.EBO);
    }
    m_loadedMeshes.clear();

    m_shader->clear();

    glfwDestroyWindow(GLFWBackendContext::getWindow(this));
    glfwTerminate();
}

void GLFWBackend::pollEvents()
{
    glfwPollEvents();
}

void GLFWBackend::beginFrame()
{
    glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFWBackend::endFrame()
{
    glfwSwapBuffers(GLFWBackendContext::getWindow(this));
}

core::MeshId GLFWBackend::loadMesh(const core::Mesh& mesh)
{
    if (auto loadResult = loadMeshToGPU(mesh); loadResult.has_value()) {
        core::MeshId meshId    = ++m_nextMeshId;
        m_loadedMeshes[meshId] = loadResult.value();
        return meshId;
    }

    return 0;
}

// TODO: Add submeshes support with materials
// TODO: Add instancing
void GLFWBackend::renderMesh(core::MeshId meshId)
{
    auto it = m_loadedMeshes.find(meshId);
    if (it == m_loadedMeshes.end()) {
        return;
    }

    const auto& meshInfo = it->second;

    m_shader->use();

    const Matrix4 model = glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));
    const Matrix4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    const Matrix4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    auto u_model      = m_shader->getUniformLocation("model");
    auto u_view       = m_shader->getUniformLocation("view");
    auto u_projection = m_shader->getUniformLocation("projection");

    m_shader->setUniform(u_model, model);
    m_shader->setUniform(u_view, view);
    m_shader->setUniform(u_projection, projection);

    glBindVertexArray(meshInfo.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(meshInfo.indicesCount), GL_UNSIGNED_INT, 0);
}

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

std::expected<GLFWBackend::MeshInfo, bool> GLFWBackend::loadMeshToGPU(const core::Mesh& mesh)
{
    MeshInfo info;

    glGenVertexArrays(1, &info.VAO);
    glGenBuffers(1, &info.VBO);
    glGenBuffers(1, &info.EBO);

    glBindVertexArray(info.VAO);

    // Set up vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, info.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(mesh.vertices.size() * sizeof(core::Vertex)),
                 mesh.vertices.data(),
                 GL_STATIC_DRAW);

    // Set up element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(mesh.subMeshes[0].indices.size() * sizeof(unsigned int)),
                 mesh.subMeshes[0].indices.data(),
                 GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(core::Vertex),
                          reinterpret_cast<void*>(offsetof(core::Vertex, position)));
    glEnableVertexAttribArray(0);

    // Vertex normals
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(core::Vertex),
                          reinterpret_cast<void*>(offsetof(core::Vertex, normal)));
    glEnableVertexAttribArray(1);

    // Vertex UVs
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(core::Vertex),
                          reinterpret_cast<void*>(offsetof(core::Vertex, uv)));
    glEnableVertexAttribArray(2);

    // Vertex colors
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(core::Vertex),
                          reinterpret_cast<void*>(offsetof(core::Vertex, color)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info.indicesCount = mesh.subMeshes[0].indices.size();

    if (hasOpenGLErrors()) {
        return std::unexpected(false);
    }

    return info;
}

} // namespace game_engine::backend
