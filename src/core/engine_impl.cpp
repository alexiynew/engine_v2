#include "engine_impl.hpp"

#include <iostream>
#include <thread>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{

inline constexpr std::chrono::nanoseconds Second = std::chrono::seconds(1);

const char* VertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aUV;
        layout(location = 3) in vec4 aColor;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec4 color;
        void main() {
            color = aColor;
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

const char* FragmentShaderSource = R"(
        #version 330 core
        in vec4 color;

        out vec4 FragColor;

        void main() {
            FragColor = color;
            //FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

} // namespace

namespace game_engine::core
{

EngineImpl::EngineImpl(std::shared_ptr<backend::Backend> backend)
    : m_backend(std::move(backend))
{
    m_modelLoader = std::make_shared<ModelLoader>();
}

EngineImpl::~EngineImpl()
{
    m_game.reset();
    m_backend.reset();
}

void EngineImpl::setGameInstance(std::shared_ptr<Game> game)
{
    m_game = std::move(game);
}

int EngineImpl::run() noexcept
{
    m_engineStartTime = getTime();
    std::cout << "EngineImpl::EngineImpl time:" << m_engineStartTime.time_since_epoch().count() << std::endl;

    try {
        const GameSettings& settings = m_game->getSettings();
        if (!m_backend->initialize(settings)) {
            return -1;
        }
        setupFrameRate(settings);
        m_backend->attachBackendObserver(*this);

        // TODO: move resource loading in separate function
        m_shader = m_backend->createShader();
        m_shader->setSource(VertexShaderSource, FragmentShaderSource);

        if (!m_shader->link()) {
            return -1;
        }

        m_game->onInitialize();

        mainLoop();

        m_backend->detachBackendObserver(*this);

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

std::shared_ptr<ModelLoader> EngineImpl::getModelLoader()
{
    return m_modelLoader;
}

std::shared_ptr<Mesh> EngineImpl::createMesh()
{
    return m_backend->createMesh();
}

// TODO: Add automatic instancing. User can call several render comands with same mesh, but different attributes.
void EngineImpl::render(const std::shared_ptr<Mesh>& mesh)
{
    m_backend->render(mesh);
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
    m_backend->beginFrame();

    m_backend->useShader(m_shader);

    float time = (static_cast<float>(m_totalFrames) * 3.14f) / 180.0f;

    using core::Uniform;
    const Uniform model      = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.5f, 1.0f, 0.0f));
    const Uniform view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    const Uniform projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_shader->setUniform("model", model);
    m_shader->setUniform("view", view);
    m_shader->setUniform("projection", projection);

    m_game->onDraw();
    m_backend->endFrame();
}

} // namespace game_engine::core
