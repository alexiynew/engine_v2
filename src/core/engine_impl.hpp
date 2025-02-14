
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
    TimePoint get_time() const noexcept override;
    bool should_stop() const noexcept override;

    void set_should_stop_flag() noexcept override;

private:
    std::unique_ptr<game_engine::backend::Backend> m_backend;
    std::unique_ptr<game_engine::Game> m_game;

    TimePoint m_engine_start_time;
    bool m_should_stop = false;

    std::chrono::nanoseconds m_target_update_time;
    std::chrono::nanoseconds m_target_frame_time;

    std::size_t m_updates            = 0;
    std::size_t m_frames             = 0;
    std::size_t m_updates_per_second = 0;
    std::size_t m_frames_per_second  = 0;

    // BackendEventHandler
    void on_keyboard_input_event(const KeyboardInputEvent& event) override;

    void on_window_resize(int width, int height) override;
    void on_window_move(int xpos, int ypos) override;
    void on_window_close() override;
    void on_window_focus(bool focused) override;
    void on_window_iconify(bool iconified) override;
    void on_window_maximize(bool maximized) override;

    void main_loop();

    void update(std::chrono::nanoseconds elapsed_time);
    void render();
};

} // namespace game_engine::core
