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

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine::backend
{

std::shared_ptr<Backend> createBackendInstance()
{
    return std::make_shared<GLFWBackend>();
}

GLFWBackend::GLFWBackend()
    : m_shutdownFuture(m_shutdownPromise.get_future())
{}

GLFWBackend::~GLFWBackend()
{
    shutdown();
}

#pragma region Backend

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

    // Create render thread before making context current
    m_renderThread = std::thread(&GLFWBackend::renderThreadFunction, this);

    if (!setupOpenGL()) {
        shutdown();
        return false;
    }

    applySettings(settings);

    return true;
}

void GLFWBackend::shutdown()
{
    if (m_shouldStop) {
        return;
    }

    submitCommand([this] {
        // Explicitly call `clear` on each mesh because they may be held by external code and might not be cleared by the destructor.
        for (auto& mesh : m_meshes) {
            mesh->clear();
        }
        // Explicitly call `clear` on each shader because they may be held by external code and might not be cleared by the destructor.
        for (auto& shader : m_shaders) {
            shader->clear();
        }
    });

    m_shouldStop = true;
    m_commandCondition.notify_one();

    if (m_renderThread.joinable()) {
        m_renderThread.join();
    }

    m_meshes.clear();
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
    submitCommand([] {
        glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
}

void GLFWBackend::endFrame()
{
    submitCommand([this] {
        // --
        glfwSwapBuffers(GLFWBackendContext::getWindow(this));
    });
}

void GLFWBackend::applySettings(const GameSettings& settings)
{
    GLFWwindow* window = GLFWBackendContext::getWindow(this);

    applyDisplayMode(settings);
    applyAntiAliasing(settings);

    glfwSetWindowTitle(window, settings.windowTitle.c_str());

    glfwSwapInterval(settings.vSync ? 1 : 0);
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

void GLFWBackend::addRenderCommand(const RenderCommand& command)
{
    std::lock_guard<std::mutex> lock(m_renderCommandsMutex);
    m_renderCommands.push_back(command);
}

void GLFWBackend::clearRenderCommands()
{
    std::lock_guard<std::mutex> lock(m_renderCommandsMutex);
    m_renderCommands.clear();
}

// TODO: Add submeshes support with materials
void GLFWBackend::executeRenderCommands()
{
    std::vector<RenderCommand> commands;
    {
        std::lock_guard<std::mutex> lock(m_renderCommandsMutex);
        commands = std::move(m_renderCommands);
        m_renderCommands.clear();
    }

    submitCommand([commands = std::move(commands)] {
        for (const auto& cmd : commands) {
            const auto openglShader = std::dynamic_pointer_cast<OpenGLShader>(cmd.shader);
            if (openglShader && openglShader->isValid()) {
                openglShader->use();

                for (const auto& uniform : cmd.uniforms) {
                    openglShader->setUniform(uniform);
                }
            }

            const auto openglMesh = std::dynamic_pointer_cast<OpenGLMesh>(cmd.mesh);
            if (openglMesh && openglMesh->isValid()) {
                if (cmd.instanceCount > 1) {
                    // TODO: Implement instancing
                } else {
                    openglMesh->render();
                }
            }
        }
    });
}

#pragma endregion

#pragma region GLWFBackend

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

template <typename F>
void GLFWBackend::submitCommand(F&& command)
{
    std::lock_guard lock(m_commandMutex);
    m_commandQueue.emplace_back(std::forward<F>(command));
    m_commandCondition.notify_one();
}

bool GLFWBackend::setupOpenGL()
{
    std::promise<bool> completionPromise;
    auto completionFuture = completionPromise.get_future();

    submitCommand([&completionPromise] {
        if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
            completionPromise.set_value(false);
        }

        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        completionPromise.set_value(true);
    });

    return completionFuture.get();
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

void GLFWBackend::renderThreadFunction()
{
    while (!m_shouldStop) {
        std::vector<std::function<void()>> commands;

        {
            std::unique_lock lock(m_commandMutex);
            m_commandCondition.wait(lock, [this] { return !m_commandQueue.empty() || m_shouldStop; });

            if (m_shouldStop) {
                break;
            }

            commands = std::move(m_commandQueue);
            m_commandQueue.clear();
        }

        try {
            glfwMakeContextCurrent(GLFWBackendContext::getWindow(this));

            for (auto& command : commands) {
                command();
            }

            // Ensure context is released
            glfwMakeContextCurrent(nullptr);
        } catch (const std::exception& e) {
            LOG_ERROR << "Render thread error: " << e.what() << std::endl;
            m_shutdownPromise.set_exception(std::current_exception());
            m_shouldStop = true;
        }
    }
}

#pragma endregion

} // namespace game_engine::backend
