#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <thread>

#include <modules/graphics/renderer.hpp>
#include <modules/render_context.hpp>

namespace game_engine::graphics
{

class OpenGLShader;
class OpenGLMesh;

class OpenGLRenderer final
    : public Renderer
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
    bool init(std::shared_ptr<const RenderContext> context) noexcept override;
    void shutdown() noexcept override;

    std::shared_ptr<graphics::Shader> createShader() override;
    std::shared_ptr<graphics::Mesh> createMesh() override;

    void addRenderCommand(const RenderCommand& command) override;
    void clearRenderCommands() override;
    void executeRenderCommands() override;

    void submit(Task task);
    std::future<void> submitSync(Task task);

private:

    void renderLoop();

    std::shared_ptr<const RenderContext> m_context;

    std::atomic<bool> m_running{false};
    std::thread m_thread;

    std::deque<std::packaged_task<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::promise<void> m_initPromise;

    std::vector<std::shared_ptr<OpenGLShader>> m_shaders;
    std::vector<std::shared_ptr<OpenGLMesh>> m_meshes;

    std::mutex m_commandsMutex;
    std::deque<RenderCommand> m_commands;
};

} // namespace game_engine::graphics
