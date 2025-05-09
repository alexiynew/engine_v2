#include "opengl_renderer.hpp"

#include <glad/glad.h>
#include <opengl_mesh.hpp>
#include <opengl_shader.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine::graphics
{

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<RenderContext> context)
    : m_context(std::move(context))
{}

OpenGLRenderer::~OpenGLRenderer()
{
    shutdown();
}

#pragma region Renderer

bool OpenGLRenderer::initialize() noexcept
{
    try {
        m_running.store(true, std::memory_order_release);
        m_thread = std::thread(&OpenGLRenderer::renderLoop, this);

        // Wait for tread to start
        m_initPromise.get_future().get();

        auto result = submitSync([this] {
            m_context->makeCurrent();

            if (gladLoadGL() == 0 || GLVersion.major != 3 || GLVersion.minor != 3) {
                throw std::runtime_error("Unsupported OpenGL version");
            }

            glEnable(GL_DEPTH_TEST);
            // TODO: enable antialiasing from settings
            glEnable(GL_MULTISAMPLE);
            glFrontFace(GL_CCW);
            glCullFace(GL_BACK);
        });

        result.get();
    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void OpenGLRenderer::shutdown() noexcept
{
    try {
        if (m_thread.joinable()) {
            if (!m_meshes.empty()) {
                auto result = submitSync([this] {
                    // Explicitly call `clear` on each mesh because they may be held by external code and might not be cleared by the destructor.
                    for (auto& mesh : m_meshes) {
                        mesh->clear();
                    }
                });
                result.get();
            }

            if (!m_shaders.empty()) {
                auto result = submitSync([this] {
                    // Explicitly call `clear` on each shader because they may be held by external code and might not be cleared by the destructor.
                    for (auto& shader : m_shaders) {
                        shader->clear();
                    }
                });
                result.get();
            }

            {
                auto result = submitSync([this] { m_context->dropCurrent(); });
                result.get();
            }

            m_running.store(false, std::memory_order_release);
            m_cv.notify_all();
            m_thread.join();
        }

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
    }

    m_meshes.clear();
    m_shaders.clear();

    m_context.reset();
}

std::shared_ptr<graphics::Shader> OpenGLRenderer::createShader()
{
    m_shaders.push_back(std::make_shared<OpenGLShader>(shared_from_this()));
    return m_shaders.back();
}

std::shared_ptr<graphics::Mesh> OpenGLRenderer::createMesh()
{
    m_meshes.push_back(std::make_shared<OpenGLMesh>(shared_from_this()));
    return m_meshes.back();
}

void OpenGLRenderer::addRenderCommand(const RenderCommand& command)
{
    std::lock_guard<std::mutex> lock(m_commandsMutex);
    m_commands.push_back(command);
}

void OpenGLRenderer::clearRenderCommands()
{
    std::lock_guard<std::mutex> lock(m_commandsMutex);
    m_commands.clear();
}

// TODO: Add submeshes support with materials
void OpenGLRenderer::executeRenderCommands()
{
    submit([] {
        glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });

    std::deque<RenderCommand> commands;
    {
        std::lock_guard<std::mutex> lock(m_commandsMutex);
        std::swap(m_commands, commands);
    }

    submit([commands = std::move(commands)] {
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

    submit([this] {
        // --
        m_context->swapBuffers();
    });
}

#pragma endregion

#pragma region OpenGLRenderer private

void OpenGLRenderer::submit(Task task)
{
    std::packaged_task<void()> packagedTask(std::move(task));

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packagedTask));
    }

    m_cv.notify_one();
}

std::future<void> OpenGLRenderer::submitSync(Task task)
{
    std::packaged_task<void()> packagedTask(std::move(task));
    auto future = packagedTask.get_future();

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packagedTask));
    }

    m_cv.notify_one();
    return future;
}

void OpenGLRenderer::renderLoop()
{
    // Signal tread start
    m_initPromise.set_value();

    while (m_running.load(std::memory_order_acquire)) {
        std::deque<std::packaged_task<void()>> tasks;

        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_running.load(std::memory_order_acquire) || !m_tasks.empty(); });

            if (!m_running) {
                break;
            }

            swap(tasks, m_tasks);
        }

        try {
            for (auto& task : tasks) {
                task();
            }

        } catch (...) {
            // TODO report error to backend;
            // report error
        }
    }
}

#pragma endregion

} // namespace game_engine::graphics

