#include "backend.hpp"

namespace game_engine::backend
{

Backend::Backend(BackendEventHandler& handler)
    : m_event_handler(handler) {
}

} // namespace game_engine::backend
