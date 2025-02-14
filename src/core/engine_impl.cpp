#include "engine_impl.hpp"

#include <iostream>
#include <thread>

namespace game_engine::core
{

EngineImpl::EngineImpl() {
    m_backend = backend::create_backend_instance(*this);
    if (!m_backend) {
        throw std::runtime_error("Backend is not created");
    }

    m_game = create_game_instance(*this);
    if (!m_game) {
        throw std::runtime_error("Game is not created");
    }

    std::chrono::nanoseconds second = std::chrono::seconds(1);
    std::size_t fps                 = 60;

    m_target_update_time = second / fps;
    m_target_frame_time  = second / fps;
}

EngineImpl::~EngineImpl() {
    m_game.reset();
    m_backend.reset();
}

int EngineImpl::run() noexcept {
    m_engine_start_time = get_time();
    std::cout << "EngineImpl::EngineImpl time:" << m_engine_start_time.time_since_epoch().count() << std::endl;

    try {
        if (!m_backend->initialize()) {
            return -1;
        }
        m_game->on_initialize();

        main_loop();

        m_game->on_shutdown();
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

EngineImpl::TimePoint EngineImpl::get_time() const noexcept {
    return std::chrono::steady_clock::now();
}

bool EngineImpl::should_stop() const noexcept {
    return m_should_stop;
}

void EngineImpl::set_should_stop_flag() noexcept {
    m_should_stop = true;
}

void EngineImpl::on_keyboard_input_event(const KeyboardInputEvent& event) {
    m_game->on_keyboard_input_event(event);
}

void EngineImpl::on_window_resize(int width, int height) {
}

void EngineImpl::on_window_move(int xpos, int ypos) {
}

void EngineImpl::on_window_close() {
    if (m_game->on_should_close()) {
        set_should_stop_flag();
    }
}

void EngineImpl::on_window_focus(bool focused) {
}

void EngineImpl::on_window_iconify(bool iconified) {
}

void EngineImpl::on_window_maximize(bool maximized) {
}

void EngineImpl::main_loop() {
    TimePoint last_time        = get_time();
    TimePoint fps_counter_time = last_time;

    std::chrono::nanoseconds updates_delta_time {0};
    std::chrono::nanoseconds frames_delta_time {0};

    while (!should_stop()) {
        m_backend->poll_events();

        const TimePoint now_time = get_time();
        auto frame_duration      = (now_time - last_time);

        // Run the required number of updates
        updates_delta_time += frame_duration;
        while (updates_delta_time >= m_target_update_time) {
            update(m_target_update_time);
            m_updates++;
            updates_delta_time -= m_target_update_time;
        }

        // Render one frame
        frames_delta_time += frame_duration;
        if (frames_delta_time >= m_target_frame_time) {
            render();
            m_frames++;
            frames_delta_time -= m_target_frame_time;
        }

        last_time = now_time;

        // Count fps and ups for one second
        if (now_time - fps_counter_time > std::chrono::seconds(1)) {
            fps_counter_time     = now_time;
            m_updates_per_second = m_updates;
            m_frames_per_second  = m_frames;
            m_updates            = 0;
            m_frames             = 0;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

void EngineImpl::update(std::chrono::nanoseconds elapsed_time) {
    m_game->on_update(elapsed_time);
}

void EngineImpl::render() {
    m_backend->begin_frame();
    m_game->on_draw();
    m_backend->end_frame();
}

} // namespace game_engine::core
