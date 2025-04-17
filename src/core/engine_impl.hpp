
#pragma once

#include <game_engine/core/engine.hpp>
#include <game_engine/game.hpp>
#include <game_engine/core/logger.hpp>

#include <backend/backend.hpp>

namespace game_engine::core
{

class EngineImpl final
    : public game_engine::core::Engine
    , private game_engine::backend::BackendObserver
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    explicit EngineImpl(std::shared_ptr<backend::Backend> backend);
    ~EngineImpl() override;

    // Engine
    TimePoint getTime() const noexcept override;
    bool shouldStop() const noexcept override;

    void setShouldStopFlag() noexcept override;

    std::shared_ptr<ModelLoader> getModelLoader() override;

    MeshId loadMesh(const Mesh& mesh) override;
    void renderMesh(MeshId meshId) override;

    void setGameInstance(std::shared_ptr<Game> game);

    int run() noexcept;

private:
    std::shared_ptr<game_engine::backend::Backend> m_backend;
    std::shared_ptr<game_engine::Game> m_game;

    std::shared_ptr<game_engine::core::ModelLoader> m_modelLoader;

    TimePoint m_engineStartTime;
    bool m_shouldStop = false;

    std::chrono::nanoseconds m_targetUpdateTime;
    std::chrono::nanoseconds m_targetFrameTime;

    std::size_t m_updates          = 0;
    std::size_t m_frames           = 0;
    std::size_t m_updatesPerSecond = 0;
    std::size_t m_framesPerSecond  = 0;

    // BackendEventHandler
    void onEvent(const KeyboardInputEvent& event) override;

    void onEvent(const WindowResizeEvent& event) override;
    void onEvent(const WindowMoveEvent& event) override;
    void onEvent(const WindowCloseEvent& event) override;
    void onEvent(const WindowFocusEvent& event) override;
    void onEvent(const WindowIconifyEvent& event) override;
    void onEvent(const WindowMaximizeEvent& event) override;

    void mainLoop();

    void update(std::chrono::nanoseconds elapsedTime);
    void render();
};

} // namespace game_engine::core
