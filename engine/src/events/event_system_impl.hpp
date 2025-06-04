#pragma once

#include <mutex>
#include <unordered_map>

#include <engine/events/event_system.hpp>

namespace game_engine
{

class EventDispatcher;

class EventSystemImpl final : public IEventSystem
{
public:

    EventSystemImpl();
    ~EventSystemImpl() override;

private:

    // IEventSystem implementation
    SubscriptionPtr SubscribeImpl(std::type_index event_type, GenericHandler handler, HandlerPriority priority) override;
    void ProcessEventImpl(std::type_index event_type, const void* event) override;

    mutable std::mutex m_mutex;
    std::unordered_map<std::type_index, std::shared_ptr<EventDispatcher>> m_dispatchers;
};

} // namespace game_engine
