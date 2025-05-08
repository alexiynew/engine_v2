#include "engine_impl.hpp"

#include <thread>

#define LOG_ERROR std::cerr
#include <iostream>

#include <modules/module_factory.hpp>

namespace
{

inline constexpr std::chrono::nanoseconds Second = std::chrono::seconds(1);

} // namespace

namespace game_engine::core
{

EngineImpl::EngineImpl()
{
    ModuleFactory<Backend>::RegisterModule();
    ModuleFactory<Renderer>::RegisterModule();

    m_backend  = ModuleFactory<Backend>::Create();
    m_renderer = ModuleFactory<Renderer>::Create(m_backend->getRenderContext());

    m_game = createGameInstance(*this);
}

EngineImpl::~EngineImpl()
{}

int EngineImpl::run() noexcept
{
    m_engineStartTime = getTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engineStartTime.time_since_epoch().count() << std::endl;

    try {
        const GameSettings& settings = m_game->getSettings();
        if (!m_backend->initialize(settings)) {
            return -1;
        }

        if (!m_renderer->initialize()) {
            return -1;
        }

        setupFrameRate(settings);

        m_backend->attachBackendObserver(*this);

        m_game->onInitialize();

        mainLoop();

        m_backend->detachBackendObserver(*this);

        m_game->onShutdown();

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

std::shared_ptr<Mesh> EngineImpl::createMesh()
{
    return m_renderer->createMesh();
}

std::shared_ptr<Shader> EngineImpl::createShader()
{
    return m_renderer->createShader();
}

void EngineImpl::render(const std::shared_ptr<Mesh>& mesh,
                        const std::shared_ptr<Shader>& shader,
                        const std::vector<Uniform>& uniforms)
{
    RenderCommand cmd;
    cmd.mesh          = mesh;
    cmd.shader        = shader;
    cmd.uniforms      = uniforms;
    cmd.instanceCount = 1;

    m_renderer->addRenderCommand(cmd);
}

void EngineImpl::onEvent(const KeyboardInputEvent& event)
{
    m_game->onKeyboardInputEvent(event);
}

// TODO: Handle window events
// TODO: Save view aspect ratio on window resize
void EngineImpl::onEvent(const WindowResizeEvent&)
{}

void EngineImpl::onEvent(const WindowMoveEvent&)
{}

void EngineImpl::onEvent(const WindowCloseEvent&)
{
    if (m_game->onShouldClose()) {
        setShouldStopFlag();
    }
}

void EngineImpl::onEvent(const WindowFocusEvent&)
{}

void EngineImpl::onEvent(const WindowIconifyEvent&)
{}

void EngineImpl::onEvent(const WindowMaximizeEvent&)
{}

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

} // namespace game_engine::core
