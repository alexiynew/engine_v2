#include <vector>

#include <engine/event_system.hpp>

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
    std::vector<std::string> leakedEvents;
    {
        std::lock_guard lock(m_mutex);
        for (const auto& [type, dispatcher] : m_dispatchers) {
            if (dispatcher->hasHandlers()) {
                leakedEvents.push_back(dispatcher->getEventTypeName());
            }
        }
    }

    if (!leakedEvents.empty()) {
        LOG_ERROR << "[EVENT SYSTEM LEAK] Detected leaked subscriptions for events:\n";
        for (const auto& name : leakedEvents) {
            LOG_ERROR << " - " << name << "\n";
        }
    }
}

#pragma endregion

} // namespace game_engine
