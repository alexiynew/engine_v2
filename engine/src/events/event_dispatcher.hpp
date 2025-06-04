#pragma once

#include <list>
#include <mutex>
#include <unordered_map>

#include <engine/events/event_system.hpp>

namespace game_engine
{

class EventDispatcher final : public std::enable_shared_from_this<EventDispatcher>
{
public:

    using HandlerId       = std::size_t;
    using SubscriptionPtr = IEventSystem::SubscriptionPtr;
    using GenericHandler  = IEventSystem::GenericHandler;

    EventDispatcher();
    ~EventDispatcher();

    SubscriptionPtr Subscribe(GenericHandler handler, HandlerPriority priority);
    void ProcessEvent(const void* event);
    void Unsubscribe(HandlerId id) noexcept;
    bool IsEmpty() const noexcept;

private:

    struct HandlerNode
    {
        HandlerId id;
        GenericHandler handler;
        HandlerPriority priority;
    };

    void InsertHandler(HandlerNode&& node);

    mutable std::mutex m_mutex;
    std::list<HandlerNode> m_handlers;
    std::unordered_map<HandlerId, typename std::list<HandlerNode>::iterator> m_handler_map;
    HandlerId m_next_id = 1;
};

} // namespace game_engine
