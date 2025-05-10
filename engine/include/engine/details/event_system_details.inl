#pragma once

#include <engine/event_system.hpp>

namespace game_engine
{
template <typename Event>
class EventSystem::Dispatcher final
    : public DispatcherBase
    , public std::enable_shared_from_this<Dispatcher<Event>>
{
public:
    using Handler   = std::function<void(const Event&)>;
    using HandlerId = std::size_t;

    struct HandlerNode
    {
        HandlerId id;
        Handler handler;
        HandlerPriority priority;
    };

    class SubscriptionImpl final : public Subscription
    {
    public:
        SubscriptionImpl(std::weak_ptr<Dispatcher> dispatcher, HandlerId id)
            : m_dispatcher(std::move(dispatcher))
            , m_id(id)
        {}

        ~SubscriptionImpl() override
        {
            unsubscribe();
        }

        SubscriptionImpl(const SubscriptionImpl&) = delete;
        SubscriptionImpl(SubscriptionImpl&&)      = default;

        SubscriptionImpl& operator=(const SubscriptionImpl&) = delete;
        SubscriptionImpl& operator=(SubscriptionImpl&&)      = default;

        void unsubscribe() const override
        {
            if (auto dispatcher = m_dispatcher.lock()) {
                dispatcher->unsubscribe(m_id);
            }
        }

    private:
        std::weak_ptr<Dispatcher> m_dispatcher;
        HandlerId m_id;
    };

    bool hasHandlers() const override
    {
        std::lock_guard lock(m_mutex);
        return !m_handlers.empty();
    }

    std::string getEventTypeName() const override
    {
        return typeid(Event).name();
    }

    SubscriptionPtr subscribe(Handler&& handler, HandlerPriority priority)
    {
        std::lock_guard lock(m_mutex);
        HandlerId id = m_nextId++;

        auto it = addHandler({id, std::move(handler), priority});
        return std::make_unique<SubscriptionImpl>(this->weak_from_this(), id);
    }

    void unsubscribe(HandlerId id)
    {
        std::lock_guard lock(m_mutex);
        if (auto mapIt = m_handlersMap.find(id); mapIt != m_handlersMap.end()) {
            m_handlers.erase(mapIt->second);
            m_handlersMap.erase(mapIt);
        }
    }

    void processEvent(const Event& event) const
    {
        std::list<HandlerNode> handlersCopy;
        {
            std::lock_guard lock(m_mutex);
            handlersCopy = m_handlers;
        }

        for (const auto& node : handlersCopy) {
            try {
                if (node.handler) {
                    node.handler(event);
                }
            } catch (...) {
                // TODO: Error handling
                throw std::current_exception();
            }
        }
    }

private:

    auto addHandler(HandlerNode&& node)
    {
        auto it = std::lower_bound(m_handlers.begin(),
                                   m_handlers.end(),
                                   node,
                                   [](const HandlerNode& a, const HandlerNode& b) {
            if (a.priority == b.priority) {
                return a.id < b.id;
            }
            return a.priority > b.priority;
        });

        auto inserted = m_handlers.insert(it, std::move(node));

        try {
            m_handlersMap.emplace(inserted->id, inserted);
        } catch (...) {
            // Rollback: Remove from the list if map insertion failed
            m_handlers.erase(inserted);
            throw;
        }

        return inserted;
    }

    mutable std::mutex m_mutex;
    std::list<HandlerNode> m_handlers;
    std::unordered_map<HandlerId, typename std::list<HandlerNode>::iterator> m_handlersMap;
    HandlerId m_nextId{1};
};

} // namespace game_engine
