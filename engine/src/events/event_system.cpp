#include <vector>

#include <engine/events/event_system.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine
{

#pragma region HandlerPriority

std::strong_ordering operator<=>(HandlerPriority a, HandlerPriority b) noexcept
{
    using EnumType = std::underlying_type_t<HandlerPriority>;
    return static_cast<EnumType>(a) <=> static_cast<EnumType>(b);
}

#pragma endregion

#pragma region EventSystem

EventSystem::~EventSystem()
{
    std::vector<std::string> leaked_events;
    {
        std::lock_guard lock(m_mutex);
        for (const auto& [type, dispatcher] : m_dispatchers) {
            if (dispatcher->HasHandlers()) {
                leaked_events.push_back(dispatcher->GetEventTypeName());
            }
        }
    }

    if (!leaked_events.empty()) {
        LOG_ERROR << "[EVENT SYSTEM LEAK] Detected leaked subscriptions for events:\n";
        for (const auto& name : leaked_events) {
            LOG_ERROR << " - " << name << "\n";
        }
    }
}

#pragma endregion

} // namespace game_engine
