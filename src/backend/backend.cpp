#include "backend.hpp"

namespace game_engine::backend
{

Backend::Backend(BackendEventHandler& handler)
    : m_eventHandler(handler) {
}

} // namespace game_engine::backend
