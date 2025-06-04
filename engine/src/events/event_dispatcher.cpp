#include "event_dispatcher.hpp"

#include <algorithm>

namespace game_engine
{
#pragma region HandlerPriority

inline auto operator<=>(HandlerPriority a, HandlerPriority b) noexcept
{
    using EnumType = std::underlying_type_t<HandlerPriority>;
    return static_cast<EnumType>(a) <=> static_cast<EnumType>(b);
}

#pragma endregion

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

EventDispatcher::EventDispatcher() = default;

EventDispatcher::~EventDispatcher() = default;

EventDispatcher::SubscriptionPtr EventDispatcher::Subscribe(GenericHandler handler, HandlerPriority priority)
{
    std::lock_guard lock(m_mutex);
    HandlerId id = m_next_id++;

    InsertHandler(HandlerNode{id, std::move(handler), priority});

    return std::make_unique<SubscriptionImpl>(this->weak_from_this(), id);
}

void EventDispatcher::ProcessEvent(const void* event)
{
    std::vector<GenericHandler> handlers;
    {
        std::lock_guard lock(m_mutex);
        handlers.reserve(m_handlers.size());
        for (const auto& node : m_handlers) {
            handlers.push_back(node.handler);
        }
    }

    for (const auto& h : handlers) {
        h(event);
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

bool EventDispatcher::IsEmpty() const noexcept
{
    std::lock_guard lock(m_mutex);
    return m_handlers.empty();
}

void EventDispatcher::InsertHandler(HandlerNode&& node)
{
    auto it = m_handlers.begin();
    while (it != m_handlers.end() && it->priority >= node.priority) {
        if (it->priority == node.priority && it->id > node.id)
            break;
        ++it;
    }
    auto inserted = m_handlers.insert(it, std::move(node));
    m_handler_map.emplace(inserted->id, inserted);
}

#pragma endregion

} // namespace game_engine
