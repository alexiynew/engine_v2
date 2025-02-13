#include "backend.hpp"

namespace game_engine::backend
{

void Backend::set_event_handler(std::weak_ptr<BackendEventHandler> handler) {
    m_event_handler = std::move(handler);
}

void Backend::notify_keyboard_input_event(const KeyboardInputEvent& event) {
    if (auto handler = m_event_handler.lock()) {
        handler->on_keyboard_input_event(event);
    }
}

void Backend::notify_window_event(const std::string& event) {
    if (auto handler = m_event_handler.lock()) {
        handler->on_window_event(event);
    }
}

} // namespace game_engine::backend
