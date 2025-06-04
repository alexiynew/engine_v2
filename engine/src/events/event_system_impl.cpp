#include "event_system_impl.hpp"

#include <events/event_dispatcher.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine
{

#pragma region EventSystemImpl

EventSystemImpl::EventSystemImpl()
{}

EventSystemImpl::~EventSystemImpl()
{
    std::vector<std::string> leaked_events;
    {
        std::lock_guard lock(m_mutex);
        for (const auto& [type, dispatcher] : m_dispatchers) {
            if (!dispatcher->IsEmpty()) {
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

void EventSystemImpl::UnsubscribeFromAll() noexcept
{
    std::lock_guard lock(m_mutex);
    m_dispatchers.clear();
}

IEventSystem::SubscriptionPtr EventSystemImpl::SubscribeImpl(std::type_index event_type, GenericHandler handler, HandlerPriority priority)
{
    std::lock_guard lock(m_mutex);

    auto it = m_dispatchers.find(event_type);
    if (it == m_dispatchers.end()) {
        it = m_dispatchers.emplace(event_type, std::make_shared<EventDispatcher>()).first;
    }

    return it->second->Subscribe(std::move(handler), priority);
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

} // namespace game_engine
