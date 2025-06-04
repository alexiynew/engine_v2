#include "event_system_impl.hpp"

#include <events/event_dispatcher.hpp>

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

#pragma region EventSystemImpl

EventSystemImpl::EventSystemImpl()
{}

EventSystemImpl::~EventSystemImpl()
{
    std::vector<std::string> leaked_events;
    {
        std::lock_guard lock(m_mutex);
        for (const auto& [type, dispatcher] : m_dispatchers) {
            if (dispatcher->HasHandlers()) {
                leaked_events.push_back(type.name());
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

#pragma region IEventSystem implementation

IEventSystem::SubscriptionPtr EventSystemImpl::SubscribeImpl(std::type_index event_type, GenericHandler handler, HandlerPriority priority)
{
    std::lock_guard lock(m_mutex);

    if (!m_dispatchers.contains(event_type)) {
        m_dispatchers.emplace(event_type, std::make_shared<EventDispatcher>());
    }

    return m_dispatchers.at(event_type)->Subscribe(std::move(handler), priority);
}

void EventSystemImpl::ProcessEventImpl(std::type_index event_type, const void* event)
{
    std::shared_ptr<EventDispatcher> dispatcher;
    {
        std::lock_guard lock(m_mutex);
        if (auto it = m_dispatchers.find(event_type); it != m_dispatchers.end()) {
            dispatcher = it->second;
        }
    }

    if (dispatcher) {
        dispatcher->ProcessEvent(event);
    }
}

#pragma endregion

#pragma endregion

} // namespace game_engine
