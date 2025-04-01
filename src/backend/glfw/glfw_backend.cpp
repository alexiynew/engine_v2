#include "glfw_backend.hpp"

#include <unordered_map>

#include <game_engine/common_types.hpp>

#include <glfw/glfw_backend_context.hpp>
#include <glfw/glfw_keyboard.hpp>
#include <glfw/opengl_mesh.hpp>
#include <glfw/opengl_shader.hpp>
#include <glfw/opengl_utils.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace game_engine::backend
{

std::shared_ptr<Backend> createBackendInstance()
{
    return std::make_shared<GLFWBackend>();
}

GLFWBackend::GLFWBackend()
{}

GLFWBackend::~GLFWBackend() = default;

bool GLFWBackend::initialize(const GameSettings& settings)
{
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(settings.resolutionWidth,
                                   settings.resolutionHeight,
                                   settings.windowTitle.c_str(),
                                   nullptr,
                                   nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    GLFWBackendContext::registerBackend(window, this);

    if (!setupOpenGL()) {
        glfwTerminate();
        return false;
    }

    applySettings(settings);

    return true;
}

void GLFWBackend::shutdown()
{
    // Explicitly call `clear` on each mesh because they may be held by external code and might not be cleared by the destructor.
    for (auto& mesh : m_meshes) {
        mesh->clear();
    }
    m_meshes.clear();

    // Explicitly call `clear` on each shader because they may be held by external code and might not be cleared by the destructor.
    for (auto& shader : m_shaders) {
        shader->clear();
    }
    m_shaders.clear();

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

void GLFWBackend::applySettings(const GameSettings& settings)
{
    GLFWwindow* window = GLFWBackendContext::getWindow(this);

    applyDisplayMode(settings);
    applyAntiAliasing(settings);

    glfwSetWindowTitle(window, settings.windowTitle.c_str());

    glfwSwapInterval(settings.vSync ? 1 : 0);
}

bool GLFWBackend::setupOpenGL()
{
    glfwMakeContextCurrent(GLFWBackendContext::getWindow(this));

    if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
        glfwTerminate();
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    return true;
}

void GLFWBackend::applyDisplayMode(const GameSettings& settings)
{
    GLFWwindow* window      = GLFWBackendContext::getWindow(this);
    GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    switch (settings.displayMode) {
        case DisplayMode::Fullscreen:
            glfwSetWindowMonitor(window,
                                 monitor,
                                 0,
                                 0,
                                 settings.resolutionWidth,
                                 settings.resolutionHeight,
                                 mode->refreshRate);
            break;
        case DisplayMode::BorderlessFullscreen:
            glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            break;
        case DisplayMode::Windowed: {
            const int xPos = (mode->width - settings.resolutionWidth) / 2;
            const int yPos = (mode->height - settings.resolutionHeight) / 2;

            glfwSetWindowMonitor(window,
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

std::shared_ptr<core::Shader> GLFWBackend::createShader()
{
    m_shaders.push_back(std::make_shared<OpenGLShader>());
    return m_shaders.back();
}

std::shared_ptr<core::Mesh> GLFWBackend::createMesh()
{
    m_meshes.push_back(std::make_shared<OpenGLMesh>());
    return m_meshes.back();
}

// TODO: Add submeshes support with materials
// TODO: Add instancing
// TODO: Add bounding box rendering
void GLFWBackend::render(const std::shared_ptr<core::Mesh>& mesh, const std::shared_ptr<core::Shader>& shader)
{
    if (shader->isValid()) {
        shader->use();
    }

    auto openglMesh = std::dynamic_pointer_cast<OpenGLMesh>(mesh);
    if (openglMesh && openglMesh->isValid()) {
        openglMesh->render();
    }
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

} // namespace game_engine::backend
