
#pragma once

#include <engine/engine.hpp>
#include <engine/game.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>
#include <modules/module_locator.hpp>

namespace game_engine
{

class EngineImpl final
    : public IEngine
    , public std::enable_shared_from_this<EngineImpl>
    , private BackendObserver
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    explicit EngineImpl(const ModuleLocator& locator);
    ~EngineImpl() override;

    // Engine
    TimePoint getTime() const noexcept override;
    bool shouldStop() const noexcept override;
    void setShouldStopFlag() noexcept override;
    std::shared_ptr<graphics::IMesh> createMesh() override;
    std::shared_ptr<graphics::IShader> createShader() override;
    void render(const std::shared_ptr<graphics::IMesh>& mesh,
                const std::shared_ptr<graphics::IShader>& shader,
                const std::vector<graphics::Uniform>& uniforms) override;

    [[nodiscard]]
    EventSystem& getEventSystem() const override;

    ReturnCode run() noexcept;

private:
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

    std::shared_ptr<backend::Backend> m_backend;
    std::shared_ptr<graphics::Renderer> m_renderer;

    std::shared_ptr<EventSystem> m_eventSystem;

    std::shared_ptr<IGame> m_game;

    TimePoint m_engineStartTime;
    bool m_shouldStop = false;

    std::chrono::nanoseconds m_targetUpdateTime{};
    std::chrono::nanoseconds m_targetFrameTime{};

    std::size_t m_updates          = 0;
    std::size_t m_frames           = 0;
    std::size_t m_updatesPerSecond = 0;
    std::size_t m_framesPerSecond  = 0;
    std::size_t m_totalFrames      = 0;
};

} // namespace game_engine
