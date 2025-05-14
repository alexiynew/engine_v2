#include "engine_impl.hpp"

#include <thread>

#define LOG_ERROR std::cerr
#include <iostream>

namespace
{
inline constexpr std::chrono::nanoseconds Second = std::chrono::seconds(1);
} // namespace

namespace game_engine
{

EngineImpl::EngineImpl(const ModuleLocator& locator)
    : m_backend(locator.get<backend::Backend>())
    , m_renderer(locator.get<graphics::Renderer>())
    , m_eventSystem(std::make_shared<EventSystem>())
    , m_game(locator.get<Game>())
{}

EngineImpl::~EngineImpl() = default;

#pragma region Engine

EngineImpl::TimePoint EngineImpl::getTime() const noexcept
{
    return std::chrono::steady_clock::now();
}

bool EngineImpl::shouldStop() const noexcept
{
    return m_shouldStop;
}

void EngineImpl::setShouldStopFlag() noexcept
{
    m_shouldStop = true;
}

std::shared_ptr<graphics::Mesh> EngineImpl::createMesh()
{
    return m_renderer->createMesh();
}

std::shared_ptr<graphics::Shader> EngineImpl::createShader()
{
    return m_renderer->createShader();
}

void EngineImpl::render(const std::shared_ptr<graphics::Mesh>& mesh,
                        const std::shared_ptr<graphics::Shader>& shader,
                        const std::vector<graphics::Uniform>& uniforms)
{
    graphics::RenderCommand cmd;
    cmd.mesh          = mesh;
    cmd.shader        = shader;
    cmd.uniforms      = uniforms;
    cmd.instanceCount = 1;

    m_renderer->addRenderCommand(cmd);
}

[[nodiscard]]
EventSystem& EngineImpl::getEventSystem() const
{
    return *m_eventSystem;
}

#pragma endregion

#pragma region EngineImpl public

int EngineImpl::run() noexcept
{
    m_engineStartTime = getTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engineStartTime.time_since_epoch().count() << std::endl;

    try {
        const GameSettings& settings = m_game->getSettings();
        if (!m_backend->init(settings)) {
            return -1;
        }

        if (!m_renderer->init(m_backend->getRenderContext())) {
            return -1;
        }

        if (!m_game->init(shared_from_this())) {
            return -1;
        }

        setupFrameRate(settings);

        m_backend->attachBackendObserver(*this);

        mainLoop();

        m_backend->detachBackendObserver(*this);

        m_game->shutdown();
        m_renderer->shutdown();
        m_backend->shutdown();

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

void EngineImpl::onEvent(const KeyboardInputEvent& event)
{
    m_eventSystem->processEvent(event);
}

// TODO: Handle window events
// TODO: Save view aspect ratio on window resize
void EngineImpl::onEvent(const WindowResizeEvent& event)
{
    m_eventSystem->processEvent(event);
}

void EngineImpl::onEvent(const WindowMoveEvent& event)
{
    m_eventSystem->processEvent(event);
}

void EngineImpl::onEvent(const WindowCloseEvent& event)
{
    m_eventSystem->processEvent(event);

    if (m_game->onShouldClose()) {
        setShouldStopFlag();
    }
}

void EngineImpl::onEvent(const WindowFocusEvent& event)
{
    m_eventSystem->processEvent(event);
}

void EngineImpl::onEvent(const WindowIconifyEvent& event)
{
    m_eventSystem->processEvent(event);
}

void EngineImpl::onEvent(const WindowMaximizeEvent& event)
{
    m_eventSystem->processEvent(event);
}

#pragma endregion

#pragma region EngineImpl private

void EngineImpl::setupFrameRate(const GameSettings& settings)
{
    m_targetUpdateTime = Second / settings.updateRate;
    m_targetFrameTime  = Second / settings.frameRate;
}

void EngineImpl::mainLoop()
{
    TimePoint lastTime       = getTime();
    TimePoint fpsCounterTime = lastTime;

    std::chrono::nanoseconds updatesDeltaTime{0};
    std::chrono::nanoseconds framesDeltaTime{0};

    while (!shouldStop()) {
        m_backend->pollEvents();

        const TimePoint nowTime = getTime();
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
            render();
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
    m_game->onUpdate(elapsedTime);
}

void EngineImpl::render()
{
    m_game->onDraw();

    m_renderer->executeRenderCommands();
    m_renderer->clearRenderCommands();
}

#pragma endregion

} // namespace game_engine
