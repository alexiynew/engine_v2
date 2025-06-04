#include "event_dispatcher.hpp"

#include <algorithm>

namespace game_engine
{

#pragma region SubscriptionImpl

class SubscriptionImpl final : public ISubscription
{
public:

    SubscriptionImpl(std::weak_ptr<EventDispatcher> dispatcher, EventDispatcher::HandlerId id)
        : m_dispatcher(std::move(dispatcher))
        , m_id(id)
    {}

    ~SubscriptionImpl() override
    {
        Unsubscribe();
    }

    void Unsubscribe() noexcept override
    {
        if (auto dp = m_dispatcher.lock()) {
            dp->Unsubscribe(m_id);
        }
    }

private:

    std::weak_ptr<EventDispatcher> m_dispatcher;
    EventDispatcher::HandlerId m_id;
};

#pragma endregion

#pragma region EventDispatcher

EventDispatcher::SubscriptionPtr EventDispatcher::Subscribe(GenericHandler handler, HandlerPriority priority)
{
    std::lock_guard lock(m_mutex);
    HandlerId id = m_next_id++;

    InsertHandler(HandlerNode{id, std::move(handler), priority});

    return std::make_unique<SubscriptionImpl>(this->weak_from_this(), id);
}

void EventDispatcher::ProcessEvent(const void* event)
{
    std::list<HandlerNode> handlers;
    {
        std::lock_guard lock(m_mutex);
        handlers = m_handlers;
    }

    for (const auto& node : handlers) {
        node.handler(event);
    }
}

void EventDispatcher::Unsubscribe(HandlerId id) noexcept
{
    std::lock_guard lock(m_mutex);
    if (auto it = m_handler_map.find(id); it != m_handler_map.end()) {
        m_handlers.erase(it->second);
        m_handler_map.erase(it);
    }
}

bool EventDispatcher::HasHandlers() const noexcept
{
    return !m_handlers.empty();
}

void EventDispatcher::InsertHandler(HandlerNode&& node)
{
    auto comp = [](const HandlerNode& a, const HandlerNode& b) { return a.priority > b.priority; };

    auto it = std::lower_bound(m_handlers.begin(), m_handlers.end(), node, comp);

    auto inserted = m_handlers.insert(it, std::move(node));
    m_handler_map.emplace(inserted->id, inserted);
}

#pragma endregion

} // namespace game_engine
