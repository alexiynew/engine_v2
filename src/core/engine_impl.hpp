
#pragma once

#include <backend/backend.hpp>
#include <game_engine/core/engine.hpp>
#include <game_engine/game.hpp>

namespace game_engine::core
{

class EngineImpl final
    : public game_engine::core::Engine
    , public game_engine::backend::BackendEventHandler
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    EngineImpl();
    ~EngineImpl() override;

    int run() noexcept;

    // Engine
    TimePoint getTime() const noexcept override;
    bool shouldStop() const noexcept override;

    void setShouldStopFlag() noexcept override;

    std::shared_ptr<ModelLoader> getModelLoader() override;

    MeshId loadMesh(const Mesh& mesh) override;
    void renderMesh(MeshId meshId) override;

private:
    std::unique_ptr<game_engine::backend::Backend> m_backend;
    std::unique_ptr<game_engine::Game> m_game;

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
    void onKeyboardInputEvent(const KeyboardInputEvent& event) override;

    void onWindowResize(int width, int height) override;
    void onWindowMove(int xpos, int ypos) override;
    void onWindowClose() override;
    void onWindowFocus(bool focused) override;
    void onWindowIconify(bool iconified) override;
    void onWindowMaximize(bool maximized) override;

    void mainLoop();

    void update(std::chrono::nanoseconds elapsedTime);
    void render();
};

} // namespace game_engine::core
