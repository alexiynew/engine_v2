
#pragma once

#include <engine/engine.hpp>
#include <engine/game.hpp>

#include <modules/backend/backend_module.hpp>
#include <modules/module_locator.hpp>

namespace game_engine
{

class ResourceManagerImpl;
class RendererImpl;

class EngineImpl final
    : public IEngine
    , public std::enable_shared_from_this<EngineImpl>
    , private IBackendObserver
{
public:

    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    explicit EngineImpl(const ModuleLocator& locator);
    ~EngineImpl() override;

    // IEngine
    TimePoint GetTime() const noexcept override;
    bool ShouldStop() const noexcept override;
    void SetShouldStopFlag() noexcept override;

    [[nodiscard]]
    std::shared_ptr<IResourceManager> GetResourceManager() const override;

    [[nodiscard]]
    std::shared_ptr<IRenderer> GetRenderer() const override;

    [[nodiscard]]
    std::shared_ptr<EventSystem> GetEventSystem() const override;

    ReturnCode run() noexcept;

private:

    // BackendEventHandler
    void OnEvent(const KeyboardInputEvent& event) override;
    void OnEvent(const WindowResizeEvent& event) override;
    void OnEvent(const WindowMoveEvent& event) override;
    void OnEvent(const WindowCloseEvent& event) override;
    void OnEvent(const WindowFocusEvent& event) override;
    void OnEvent(const WindowIconifyEvent& event) override;
    void OnEvent(const WindowMaximizeEvent& event) override;

    void setupFrameRate(const GameSettings& settings);
    void mainLoop();

    void Update(std::chrono::nanoseconds elapsedTime);
    void Render();

    std::shared_ptr<backend::IBackendModule> m_backend;

    std::shared_ptr<RendererImpl> m_renderer;
    std::shared_ptr<ResourceManagerImpl> m_resource_manager;
    std::shared_ptr<EventSystem> m_event_system;

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
