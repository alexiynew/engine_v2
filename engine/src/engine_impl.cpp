#include "engine_impl.hpp"

#include <thread>

#include <graphics/renderer_impl.hpp>
#include <resource_management/resource_manager_impl.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace
{
inline constexpr std::chrono::nanoseconds Second = std::chrono::seconds(1);
} // namespace

namespace game_engine
{

EngineImpl::EngineImpl(const ModuleLocator& locator)
    : m_backend(locator.Get<backend::IBackendModule>())
    , m_renderer(std::make_shared<RendererImpl>(locator))
    , m_resource_manager(std::make_shared<ResourceManagerImpl>())
    , m_event_system(std::make_shared<EventSystem>())
    , m_game(locator.Get<IGame>())
{}

EngineImpl::~EngineImpl() = default;

#pragma region Engine

EngineImpl::TimePoint EngineImpl::GetTime() const noexcept
{
    return std::chrono::steady_clock::now();
}

bool EngineImpl::ShouldStop() const noexcept
{
    return m_should_stop;
}

void EngineImpl::SetShouldStopFlag() noexcept
{
    m_should_stop = true;
}

[[nodiscard]]
std::shared_ptr<IResourceManager> EngineImpl::GetResourceManager() const
{
    return m_resource_manager;
}

[[nodiscard]]
std::shared_ptr<IRenderer> EngineImpl::GetRenderer() const
{
    // Performs an explicit derived-to-base pointer conversion to bypass const correctness.
    return std::static_pointer_cast<IRenderer>(m_renderer);
}

[[nodiscard]]
std::shared_ptr<EventSystem> EngineImpl::GetEventSystem() const
{
    return m_event_system;
}

//void EngineImpl::Render(const std::shared_ptr<graphics::IMesh>& mesh,
//                        const std::shared_ptr<graphics::IShader>& shader,
//                        const std::vector<graphics::Uniform>& uniforms)
//{
//    graphics::RenderCommand cmd;
//    cmd.mesh           = mesh;
//    cmd.shader         = shader;
//    cmd.uniforms       = uniforms;
//    cmd.instance_count = 1;
//
//    m_renderer->AddRenderCommand(cmd);
//}

#pragma endregion

#pragma region EngineImpl public

int EngineImpl::Run() noexcept
{
    m_engine_start_time = GetTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engine_start_time.time_since_epoch().count() << std::endl;

    try {
        const GameSettings& settings = m_game->GetSettings();
        if (!m_backend->Init(settings)) {
            return -1;
        }

        if (!m_renderer->Init()) {
            return -1;
        }

        if (!m_game->Init(shared_from_this())) {
            return -1;
        }

        SetupFrameRate(settings);

        m_backend->AttachBackendObserver(*this);

        MainLoop();

        m_backend->DetachBackendObserver(*this);

        m_game->Shutdown();
        m_renderer->Shutdown();
        m_backend->Shutdown();

        if (m_game.use_count() != 1) {
            LOG_ERROR << "Game instance leaked. Uses: " << m_game.use_count() << std::endl;
        }

        if (m_renderer.use_count() != 1) {
            LOG_ERROR << "Renderer instance leaked. Uses: " << m_renderer.use_count() << std::endl;
        }

        if (m_backend.use_count() != 1) {
            LOG_ERROR << "Backend instance leaked. Uses: " << m_backend.use_count() << std::endl;
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return -1;
    }

    return 0;
}

#pragma endregion

#pragma region BackendEventHandler

void EngineImpl::OnEvent(const KeyboardInputEvent& event)
{
    m_event_system->ProcessEvent(event);
}

// TODO: Handle window events
// TODO: Save view aspect ratio on window resize
void EngineImpl::OnEvent(const WindowResizeEvent& event)
{
    m_event_system->ProcessEvent(event);
}

void EngineImpl::OnEvent(const WindowMoveEvent& event)
{
    m_event_system->ProcessEvent(event);
}

void EngineImpl::OnEvent(const WindowCloseEvent& event)
{
    m_event_system->ProcessEvent(event);

    if (m_game->OnShouldClose()) {
        SetShouldStopFlag();
    }
}

void EngineImpl::OnEvent(const WindowFocusEvent& event)
{
    m_event_system->ProcessEvent(event);
}

void EngineImpl::OnEvent(const WindowIconifyEvent& event)
{
    m_event_system->ProcessEvent(event);
}

void EngineImpl::OnEvent(const WindowMaximizeEvent& event)
{
    m_event_system->ProcessEvent(event);
}

#pragma endregion

#pragma region EngineImpl private

void EngineImpl::SetupFrameRate(const GameSettings& settings)
{
    m_target_update_time = Second / settings.update_rate;
    m_target_frame_time  = Second / settings.frame_rate;
}

void EngineImpl::MainLoop()
{
    TimePoint last_time        = GetTime();
    TimePoint fps_counter_time = last_time;

    std::chrono::nanoseconds updates_delta_time{0};
    std::chrono::nanoseconds frames_delta_time{0};

    while (!ShouldStop()) {
        m_backend->PollEvents();

        const TimePoint now_time = GetTime();
        auto frame_duration      = (now_time - last_time);

        // Run the required number of updates
        updates_delta_time += frame_duration;
        while (updates_delta_time >= m_target_update_time) {
            Update(m_target_update_time);
            m_updates++;
            updates_delta_time -= m_target_update_time;
        }

        // Render one frame
        frames_delta_time += frame_duration;
        if (frames_delta_time >= m_target_frame_time) {
            Render();
            m_frames++;
            m_total_frames++;
            frames_delta_time -= m_target_frame_time;
        }

        last_time = now_time;

        // Count fps and ups for one second
        if (now_time - fps_counter_time > std::chrono::seconds(1)) {
            fps_counter_time     = now_time;
            m_updates_per_second = m_updates;
            m_frames_per_second  = m_frames;
            m_updates            = 0;
            m_frames             = 0;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

void EngineImpl::Update(std::chrono::nanoseconds elapsed_time)
{
    m_game->OnUpdate(elapsed_time);
}

void EngineImpl::Render()
{
    m_game->OnDraw();

    m_renderer->EndFrame();
}

#pragma endregion

} // namespace game_engine
