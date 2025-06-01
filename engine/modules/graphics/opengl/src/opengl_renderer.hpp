#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <thread>

#include <modules/graphics/renderer.hpp>

namespace game_engine::graphics
{

class OpenGLShader;
class OpenGLMesh;

class OpenGLRenderer final
    : public IRendererModule
    , public std::enable_shared_from_this<OpenGLRenderer>
{
public:
    using Task = std::function<void()>;

    OpenGLRenderer();
    ~OpenGLRenderer() override;

    OpenGLRenderer(const OpenGLRenderer&)  = delete;
    OpenGLRenderer(OpenGLRenderer&& other) = delete;

    OpenGLRenderer& operator=(const OpenGLRenderer&)  = delete;
    OpenGLRenderer& operator=(OpenGLRenderer&& other) = delete;

    // Renderer
    bool Init(std::shared_ptr<IRenderContext> context) noexcept override;
    void Shutdown() noexcept override;

    // std::shared_ptr<OpenGLShader> CreateShader();
    // std::shared_ptr<OpenGLMesh> CreateMesh();

    // void AddRenderCommand(const RenderCommand& command);
    // void ClearRenderCommands();
    // void ExecuteRenderCommands();

    void Submit(Task task);
    std::future<void> SubmitSync(Task task);

private:

    void RenderLoop();

    std::shared_ptr<IRenderContext> m_context;

    std::atomic<bool> m_running{false};
    std::thread m_thread;

    std::deque<std::packaged_task<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::promise<void> m_init_promise;

    std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
    std::vector<std::shared_ptr<OpenGLMesh>> m_meshes;

    std::mutex m_commands_mutex;
    //std::deque<RenderCommand> m_commands;
};

} // namespace game_engine::graphics
