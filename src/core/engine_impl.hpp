
#pragma once

#include <backend/backend.hpp>
#include <game_engine/core/engine.hpp>

namespace game_engine::core
{

class EngineImpl final
    : public game_engine::core::Engine
    , public game_engine::backend::BackendEventHandler
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    EngineImpl(std::shared_ptr<game_engine::backend::Backend> backend);
    ~EngineImpl() override;

    int run() noexcept override;

    TimePoint get_time() const noexcept override;
    bool should_stop() const noexcept override;

    void set_should_stop_flag() noexcept override;

private:
    // BackendEventHandler
    void on_keyboard_input_event(const KeyboardInputEvent& event) override;
    void on_window_event(const std::string& event) override;

    void main_loop();

    void update(std::chrono::nanoseconds elapsed_time);
    void render();

    std::shared_ptr<game_engine::backend::Backend> m_backend;

    TimePoint m_engine_start_time;
    bool m_should_stop = false;

    std::chrono::nanoseconds m_target_update_time;
    std::chrono::nanoseconds m_target_frame_time;

    std::size_t m_updates            = 0;
    std::size_t m_frames             = 0;
    std::size_t m_updates_per_second = 0;
    std::size_t m_frames_per_second  = 0;
};

} // namespace game_engine::core
