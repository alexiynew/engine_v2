#include "engine_impl.hpp"

#include <iostream>
#include <thread>

namespace game_engine::core
{

EngineImpl::EngineImpl() {
    m_backend = backend::createBackendInstance(*this);
    if (!m_backend) {
        throw std::runtime_error("Backend is not created");
    }

    m_game = createGameInstance(*this);
    if (!m_game) {
        throw std::runtime_error("Game is not created");
    }

    m_modelLoader = std::make_shared<ModelLoader>();

    std::chrono::nanoseconds second = std::chrono::seconds(1);
    std::size_t fps                 = 60;

    m_targetUpdateTime = second / fps;
    m_targetFrameTime  = second / fps;
}

EngineImpl::~EngineImpl() {
    m_game.reset();
    m_backend.reset();
}

int EngineImpl::run() noexcept {
    m_engineStartTime = getTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engineStartTime.time_since_epoch().count() << std::endl;

    try {
        if (!m_backend->initialize()) {
            return -1;
        }
        m_game->onInitialize();

        mainLoop();

        m_game->onShutdown();
        m_backend->shutdown();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return -1;
    }

    return 0;
}

EngineImpl::TimePoint EngineImpl::getTime() const noexcept {
    return std::chrono::steady_clock::now();
}

bool EngineImpl::shouldStop() const noexcept {
    return m_shouldStop;
}

void EngineImpl::setShouldStopFlag() noexcept {
    m_shouldStop = true;
}

std::shared_ptr<ModelLoader> EngineImpl::getModelLoader() {
    return m_modelLoader;
}

MeshId EngineImpl::loadMesh(const Mesh& mesh) {
    return m_backend->loadMesh(mesh);
}

// TODO: Add automatic instancing. User can call several render comands with same mesh, but different attributes.
void EngineImpl::renderMesh(MeshId meshId) {
    m_backend->renderMesh(meshId);
}

void EngineImpl::onKeyboardInputEvent(const KeyboardInputEvent& event) {
    m_game->onKeyboardInputEvent(event);
}

void EngineImpl::onWindowResize(int width, int height) {
}

void EngineImpl::onWindowMove(int xpos, int ypos) {
}

void EngineImpl::onWindowClose() {
    if (m_game->onShouldClose()) {
        setShouldStopFlag();
    }
}

void EngineImpl::onWindowFocus(bool focused) {
}

void EngineImpl::onWindowIconify(bool iconified) {
}

void EngineImpl::onWindowMaximize(bool maximized) {
}

void EngineImpl::mainLoop() {
    TimePoint lastTime       = getTime();
    TimePoint fpsCounterTime = lastTime;

    std::chrono::nanoseconds updatesDeltaTime {0};
    std::chrono::nanoseconds framesDeltaTime {0};

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

void EngineImpl::update(std::chrono::nanoseconds elapsedTime) {
    m_game->onUpdate(elapsedTime);
}

void EngineImpl::render() {
    m_backend->beginFrame();
    m_game->onDraw();
    m_backend->endFrame();
}

} // namespace game_engine::core
