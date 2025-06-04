#include "renderer_impl.hpp"

#include <future>

#include <modules/backend/backend_module.hpp>
#include <modules/graphics/render_command.hpp>
#include <modules/graphics/renderer_module.hpp>
#include <modules/module_locator.hpp>

#include <graphics/render_context_impl.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine
{

RendererImpl::RendererImpl(const ModuleLocator& locator)
    : m_context(std::make_shared<RenderContextImpl>(locator.Get<backend::IBackendModule>()))
    , m_renderer_module(locator.Get<graphics::IRendererModule>())
{}

RendererImpl::~RendererImpl()
{
    Shutdown();
}

#pragma region IRenderer implementation

bool RendererImpl::Load(const std::shared_ptr<IMesh>& mesh)
{
    if (!m_running) {
        LOG_ERROR << "Renderer not initialized" << std::endl;
        return false;
    }

    try {
        auto result = SubmitWithResult<bool>([this, &mesh] { return m_renderer_module->Load(mesh); });
        result.get();

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool RendererImpl::Load(const std::shared_ptr<IShader>& shader)
{
    if (!m_running) {
        LOG_ERROR << "Renderer not initialized" << std::endl;
        return false;
    }

    try {
        auto result = SubmitWithResult<bool>([this, &shader] { return m_renderer_module->Load(shader); });
        result.get();

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool RendererImpl::Load(const std::shared_ptr<ITexture>& texture)
{
    if (!m_running) {
        LOG_ERROR << "Renderer not initialized" << std::endl;
        return false;
    }

    try {
        auto result = SubmitWithResult<bool>([this, &texture] { return m_renderer_module->Load(texture); });
        result.get();

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void RendererImpl::Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IShader>& shader, std::vector<Property> properties)
{
    graphics::RenderCommand cmd{
        .mesh           = mesh->GetId(),
        .shader         = shader->GetId(),
        .properties     = std::move(properties),
        .instance_count = 1,
    };

    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_commands.push_back(std::move(cmd));
}

void RendererImpl::Render(const std::shared_ptr<IMesh>& mesh, const std::shared_ptr<IMaterial>& material)
{}

void RendererImpl::EndFrame()
{
    using namespace graphics;

    try {
        std::deque<RenderCommand> commands;
        {
            std::lock_guard<std::mutex> lock(m_commands_mutex);
            std::swap(m_commands, commands);
        }

        Submit([this, commands = std::move(commands)] {
            m_renderer_module->Execute(BeginFrameCommand{});

            for (const auto& cmd : commands) {
                m_renderer_module->Execute(cmd);
            }

            m_renderer_module->Execute(EndFrameCommand{});

            m_context->SwapBuffers();
        });

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
    }
}

#pragma endregion

#pragma region RendererImpl methods

bool RendererImpl::Init()
{
    if (m_running) {
        return true;
    }

    try {
        m_running = true;
        m_thread  = std::thread(&RendererImpl::RenderLoop, this);

        // Wait tender thread to start
        m_init_promise.get_future().wait();

        auto result = SubmitWithResult<bool>([this] {
            m_context->MakeCurrent();
            return m_renderer_module->Init();
        });
        result.get();

    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void RendererImpl::Shutdown()
{
    if (!m_running) {
        return;
    }

    try {
        if (m_thread.joinable() && m_renderer_module && m_context) {
            auto result = SubmitWithResult<bool>([this] {
                m_renderer_module->Shutdown();
                m_context->DropCurrent();

                return true;
            });
            result.get();

            m_running = false;
            m_cv.notify_all();
            m_thread.join();
        }
    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
    }

    m_renderer_module.reset();
    m_context.reset();
}

#pragma endregion

#pragma region RendererImpl private methods

void RendererImpl::Submit(Task task)
{
    std::lock_guard lock(m_mutex);
    m_tasks.push_back(std::move(task));

    m_cv.notify_one();
}

template <typename TResult>
[[nodiscard]]
std::future<TResult> RendererImpl::SubmitWithResult(std::function<TResult()> task)
{
    auto promise = std::make_shared<std::promise<TResult>>();
    auto future  = promise->get_future();

    Submit([p = std::move(promise), t = std::move(task)]() mutable {
        try {
            p->set_value(t());
        } catch (...) {
            p->set_exception(std::current_exception());
        }
    });

    return future;
}

void RendererImpl::RenderLoop()
{
    // Signal tread start
    m_init_promise.set_value();

    while (m_running) {
        std::deque<Task> tasks;

        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_running || !m_tasks.empty(); });

            if (!m_running) {
                break;
            }

            swap(tasks, m_tasks);
        }

        try {
            for (auto& task : tasks) {
                task();
            }

        } catch (std::exception& e) {
            LOG_ERROR << "Exception: " << e.what() << std::endl;
        } catch (...) {
            LOG_ERROR << "Unknown Exception" << std::endl;
        }
    }
}

#pragma endregion

} // namespace game_engine
