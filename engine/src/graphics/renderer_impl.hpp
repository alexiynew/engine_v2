#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

#include <engine/graphics/renderer.hpp>

#include <modules/graphics/render_command.hpp>

// Forward declarations
namespace game_engine
{
class ModuleLocator;
class RenderContextImpl;

namespace graphics
{
class IRendererModule;
} // namespace graphics
} // namespace game_engine

namespace game_engine
{

class RendererImpl final : public IRenderer
{
public:

    explicit RendererImpl(const ModuleLocator& locator);
    ~RendererImpl() override;

    RendererImpl(const RendererImpl&) = delete;
    RendererImpl(RendererImpl&&)      = delete;

    RendererImpl& operator=(const RendererImpl&) = delete;
    RendererImpl& operator=(RendererImpl&&)      = delete;

    // IRenderer implementation
    bool Load(const std::shared_ptr<IMesh>& mesh) override;
    bool Load(const std::shared_ptr<IShader>& shader) override;
    bool Load(const std::shared_ptr<ITexture>& texture) override;

    void Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IShader>& shader, std::vector<Property> properties) override;
    void Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material) override;

    void EndFrame() override;

    // RendererImpl methods
    bool Init();
    void Shutdown();

private:

    using Task = std::function<void()>;

    void Submit(Task task);

    template <typename TResult>
    [[nodiscard]]
    std::future<TResult> SubmitWithResult(std::function<TResult()> task);

    void RenderLoop();

    std::shared_ptr<RenderContextImpl> m_context;

    std::shared_ptr<graphics::IRendererModule> m_renderer_module;

    std::atomic<bool> m_running = false;
    std::thread m_thread;

    std::deque<Task> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::promise<void> m_init_promise;

    std::mutex m_commands_mutex;
    std::deque<graphics::RenderCommand> m_commands;
};

} // namespace game_engine
