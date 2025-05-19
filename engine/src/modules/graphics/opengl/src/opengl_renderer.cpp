#include "opengl_renderer.hpp"

#include <glad/glad.h>
#include <opengl_mesh.hpp>
#include <opengl_shader.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine::graphics
{

OpenGLRenderer::OpenGLRenderer()
{}

OpenGLRenderer::~OpenGLRenderer()
{
    Shutdown();
}

#pragma region Renderer

bool OpenGLRenderer::Init(std::shared_ptr<const IRenderContext> context) noexcept
{
    try {
        m_context = std::move(context);

        m_running.store(true, std::memory_order_release);
        m_thread = std::thread(&OpenGLRenderer::RenderLoop, this);

        // Wait for tread to start
        m_init_promise.get_future().get();

        auto result = SubmitSync([this] {
            m_context->MakeCurrent();

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

void OpenGLRenderer::Shutdown() noexcept
{
    try {
        if (m_thread.joinable()) {
            if (!m_meshes.empty()) {
                auto result = SubmitSync([this] {
                    // Explicitly call `clear` on each mesh because they may be held by external code and might not be cleared by the destructor.
                    for (auto& mesh : m_meshes) {
                        mesh->Clear();
                    }
                });
                result.get();
            }

            if (!m_shaders.empty()) {
                auto result = SubmitSync([this] {
                    // Explicitly call `clear` on each shader because they may be held by external code and might not be cleared by the destructor.
                    for (auto& shader : m_shaders) {
                        shader->Clear();
                    }
                });
                result.get();
            }

            {
                auto result = SubmitSync([this] { m_context->DropCurrent(); });
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

std::shared_ptr<graphics::IShader> OpenGLRenderer::CreateShader()
{
    m_shaders.push_back(std::make_shared<OpenGLShader>(shared_from_this()));
    return m_shaders.back();
}

std::shared_ptr<graphics::IMesh> OpenGLRenderer::CreateMesh()
{
    m_meshes.push_back(std::make_shared<OpenGLMesh>(shared_from_this()));
    return m_meshes.back();
}

void OpenGLRenderer::AddRenderCommand(const RenderCommand& command)
{
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_commands.push_back(command);
}

void OpenGLRenderer::ClearRenderCommands()
{
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_commands.clear();
}

// TODO: Add submeshes support with materials
void OpenGLRenderer::ExecuteRenderCommands()
{
    Submit([] {
        glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });

    std::deque<RenderCommand> commands;
    {
        std::lock_guard<std::mutex> lock(m_commands_mutex);
        std::swap(m_commands, commands);
    }

    Submit([commands = std::move(commands)] {
        for (const auto& cmd : commands) {
            const auto opengl_shader = std::dynamic_pointer_cast<OpenGLShader>(cmd.shader);
            if (opengl_shader && opengl_shader->IsValid()) {
                opengl_shader->Use();

                for (const auto& uniform : cmd.uniforms) {
                    opengl_shader->SetUniform(uniform);
                }
            }

            const auto opengl_mesh = std::dynamic_pointer_cast<OpenGLMesh>(cmd.mesh);
            if (opengl_mesh && opengl_mesh->IsValid()) {
                if (cmd.instance_count > 1) {
                    // TODO: Implement instancing
                } else {
                    opengl_mesh->Render();
                }
            }
        }
    });

    Submit([this] {
        // --
        m_context->SwapBuffers();
    });
}

#pragma endregion

#pragma region OpenGLRenderer private

void OpenGLRenderer::Submit(Task task)
{
    std::packaged_task<void()> packaged_task(std::move(task));

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packaged_task));
    }

    m_cv.notify_one();
}

std::future<void> OpenGLRenderer::SubmitSync(Task task)
{
    std::packaged_task<void()> packaged_task(std::move(task));
    auto future = packaged_task.get_future();

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packaged_task));
    }

    m_cv.notify_one();
    return future;
}

void OpenGLRenderer::RenderLoop()
{
    // Signal tread start
    m_init_promise.set_value();

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

