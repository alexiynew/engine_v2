#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace game_engine
{
enum class HandlerPriority : std::uint8_t
{
    Whenever         = 0,
    UrgentButCanVibe = 1,
    RedPanic         = 2,
};

inline auto operator<=>(HandlerPriority a, HandlerPriority b) noexcept
{
    using EnumType = std::underlying_type_t<HandlerPriority>;
    return static_cast<EnumType>(a) <=> static_cast<EnumType>(b);
}

class EventSystem final
{
public:

    // TODO: Report about leaked subscriptions in destructor
    ~EventSystem() = default;

    // Types
    class SubscriptionHandle
    {
    public:
        virtual ~SubscriptionHandle() = default;
        virtual void unsubscribe()    = 0;
    };

    using SubscriptionPtr = std::unique_ptr<SubscriptionHandle>;

    // Methods
    template <typename Event>
    SubscriptionPtr subscribe(std::function<void(const Event&)> handler,
                              HandlerPriority priority = HandlerPriority::Whenever)
    {
        auto dispatcher = getDispatcher<Event>();
        return dispatcher->subscribe(std::move(handler), priority);
    }

    template <typename Event>
    void processEvent(const Event& event)
    {
        if (auto dispatcher = getDispatcher<Event>()) {
            dispatcher->processEvent(event);
        }
    }

private:
    template <typename Event>
    class Dispatcher;

    template <typename Event>
    std::shared_ptr<Dispatcher<Event>> getDispatcher()
    {
        const std::type_index type = typeid(Event);

        std::lock_guard lock(m_mutex);
        if (auto it = m_dispatchers.find(type); it != m_dispatchers.end()) {
            return std::static_pointer_cast<Dispatcher<Event>>(it->second);
        }

        auto dispatcher     = std::make_shared<Dispatcher<Event>>();
        m_dispatchers[type] = dispatcher;
        return dispatcher;
    }

    std::mutex m_mutex;
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_dispatchers;
};

template <typename Event>
class EventSystem::Dispatcher final : public std::enable_shared_from_this<Dispatcher<Event>>
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

    class Subscription final : public SubscriptionHandle
    {
    public:
        Subscription(std::weak_ptr<Dispatcher> dispatcher, HandlerId id)
            : m_dispatcher(std::move(dispatcher))
            , m_id(id)
        {}

        void unsubscribe() override
        {
            if (auto dispatcher = m_dispatcher.lock()) {
                dispatcher->unsubscribe(m_id);
            }
        }

    private:
        std::weak_ptr<Dispatcher> m_dispatcher;
        HandlerId m_id;
    };

    SubscriptionPtr subscribe(Handler&& handler, HandlerPriority priority)
    {
        std::lock_guard lock(m_mutex);
        HandlerId id = m_nextId++;

        auto it = addHandler({id, std::move(handler), priority});
        return std::make_unique<Subscription>(weak_from_this(), id);
    }

    void unsubscribe(HandlerId id)
    {
        std::lock_guard lock(m_mutex);
        if (auto mapIt = m_handlersMap.find(id); mapIt != m_handlersMap.end()) {
            m_handlers.erase(mapIt->second);
            m_handlersMap.erase(mapIt);
        }
    }

    void processEvent(const Event& event)
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
            }
        }
    }

private:

    auto addHandler(HandlerNode node)
    {
        auto it = std::lower_bound(m_handlers.begin(),
                                   m_handlers.end(),
                                   node,
                                   [](const HandlerNode& a, const HandlerNode& b) {
            if (a.priority == b.priority) {
                return a.id < b.id;
            }
            return a.priority < b.priority;
        });

        auto inserted          = m_handlers.insert(it, std::move(node));
        m_handlersMap[node.id] = inserted;
        return inserted;
    }

    std::mutex m_mutex;
    std::list<HandlerNode> m_handlers;
    std::unordered_map<HandlerId, typename std::list<HandlerNode>::iterator> m_handlersMap;
    std::atomic<HandlerId> m_nextId{1};
};

} // namespace game_engine
