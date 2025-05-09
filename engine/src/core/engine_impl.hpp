
#pragma once

#include <engine/engine.hpp>
#include <engine/game.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>

namespace game_engine
{

class EngineImpl final
    : public Engine
    , private BackendObserver
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    EngineImpl();
    ~EngineImpl() override;

    // Engine
    TimePoint getTime() const noexcept override;
    bool shouldStop() const noexcept override;

    void setShouldStopFlag() noexcept override;

    std::shared_ptr<graphics::Mesh> createMesh() override;
    std::shared_ptr<graphics::Shader> createShader() override;

    void render(const std::shared_ptr<graphics::Mesh>& mesh,
                const std::shared_ptr<graphics::Shader>& shader,
                const std::vector<graphics::Uniform>& uniforms) override;

    ReturnCode run() noexcept override;

private:
    std::shared_ptr<backend::Backend> m_backend;
    std::shared_ptr<graphics::Renderer> m_renderer;
    std::shared_ptr<Game> m_game;

    TimePoint m_engineStartTime;
    bool m_shouldStop = false;

    std::chrono::nanoseconds m_targetUpdateTime;
    std::chrono::nanoseconds m_targetFrameTime;

    std::size_t m_updates          = 0;
    std::size_t m_frames           = 0;
    std::size_t m_updatesPerSecond = 0;
    std::size_t m_framesPerSecond  = 0;
    std::size_t m_totalFrames      = 0;

    // BackendEventHandler
    void onEvent(const KeyboardInputEvent& event) override;

    void onEvent(const WindowResizeEvent& event) override;
    void onEvent(const WindowMoveEvent& event) override;
    void onEvent(const WindowCloseEvent& event) override;
    void onEvent(const WindowFocusEvent& event) override;
    void onEvent(const WindowIconifyEvent& event) override;
    void onEvent(const WindowMaximizeEvent& event) override;

    void setupFrameRate(const GameSettings& settings);
    void mainLoop();

    void update(std::chrono::nanoseconds elapsedTime);
    void render();
};

} // namespace game_engine
