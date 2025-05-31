#include "engine_impl.hpp"

#include <thread>

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
    , m_renderer(locator.Get<graphics::IRendererModule>())
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
    return m_shouldStop;
}

void EngineImpl::SetShouldStopFlag() noexcept
{
    m_shouldStop = true;
}

[[nodiscard]]
std::shared_ptr<IResourceManager> EngineImpl::GetResourceManager() const
{
    return m_resource_manager;
}

[[nodiscard]]
std::shared_ptr<IRenderer> EngineImpl::GetRenderer() const
{
    return m_renderer;
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

int EngineImpl::run() noexcept
{
    m_engineStartTime = GetTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engineStartTime.time_since_epoch().count() << std::endl;

    try {
        const GameSettings& settings = m_game->GetSettings();
        if (!m_backend->Init(settings)) {
            return -1;
        }

        if (!m_renderer->Init(m_backend->GetRenderContext())) {
            return -1;
        }

        if (!m_game->Init(shared_from_this())) {
            return -1;
        }

        setupFrameRate(settings);

        m_backend->AttachBackendObserver(*this);

        mainLoop();

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

void EngineImpl::setupFrameRate(const GameSettings& settings)
{
    m_targetUpdateTime = Second / settings.update_rate;
    m_targetFrameTime  = Second / settings.frame_rate;
}

void EngineImpl::mainLoop()
{
    TimePoint lastTime       = GetTime();
    TimePoint fpsCounterTime = lastTime;

    std::chrono::nanoseconds updatesDeltaTime{0};
    std::chrono::nanoseconds framesDeltaTime{0};

    while (!ShouldStop()) {
        m_backend->PollEvents();

        const TimePoint nowTime = GetTime();
        auto frameDuration      = (nowTime - lastTime);

        // Run the required number of updates
        updatesDeltaTime += frameDuration;
        while (updatesDeltaTime >= m_targetUpdateTime) {
            update(m_targetUpdateTime);
            m_updates++;
            updatesDeltaTime -= m_targetUpdateTime;
        }

        // Render one frame
        framesDeltaTime += frameDuration;
        if (framesDeltaTime >= m_targetFrameTime) {
            Render();
            m_frames++;
            m_totalFrames++;
            framesDeltaTime -= m_targetFrameTime;
        }

        lastTime = nowTime;

        // Count fps and ups for one second
        if (nowTime - fpsCounterTime > std::chrono::seconds(1)) {
            fpsCounterTime     = nowTime;
            m_updatesPerSecond = m_updates;
            m_framesPerSecond  = m_frames;
            m_updates          = 0;
            m_frames           = 0;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

void EngineImpl::update(std::chrono::nanoseconds elapsedTime)
{
    m_game->OnUpdate(elapsedTime);
}

void EngineImpl::Render()
{
    m_game->OnDraw();

    m_renderer->ExecuteRenderCommands();
    m_renderer->ClearRenderCommands();
}

#pragma endregion

} // namespace game_engine
