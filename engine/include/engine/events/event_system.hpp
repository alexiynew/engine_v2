#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

namespace game_engine
{

/// @brief Priority levels for event handlers. Higher priority handlers execute first.
enum class HandlerPriority : std::uint8_t
{
    Whenever         = 0,
    UrgentButCanVibe = 1,
    RedPanic         = 2,
};

std::strong_ordering operator<=>(HandlerPriority a, HandlerPriority b) noexcept;

/// @brief Manages event subscriptions and dispatching. Thread-safe and supports multiple event types.
class EventSystem final
{
public:

    /// @brief Base class for event subscription. Unsubscribes automatically on destruction.
    class ISubscription
    {
    public:

        ISubscription()          = default;
        virtual ~ISubscription() = default;

        ISubscription(const ISubscription&) = delete;
        ISubscription(ISubscription&&)      = default;

        ISubscription& operator=(const ISubscription&) = delete;
        ISubscription& operator=(ISubscription&&)      = default;

        /// @brief Unsubscribes the associated handler. Called automatically when the handle is destroyed.
        virtual void Unsubscribe() const = 0;
    };

    using SubscriptionPtr = std::unique_ptr<ISubscription>;

    EventSystem() = default;
    ~EventSystem();

    /// @brief Subscribes a handler to events of type `Event`.
    /// @tparam Event      The event type to subscribe to.
    /// @param handler     Callback function to invoke when the event is dispatched.
    /// @param priority    Priority of the handler (default: HandlerPriority::Whenever).
    /// @returns           A subscription handle for unsubscribing.
    template <typename TEvent>
    [[nodiscard]]
    SubscriptionPtr Subscribe(std::function<void(const TEvent&)> handler, HandlerPriority priority = HandlerPriority::Whenever)
    {
        return GetDispatcher<TEvent>()->Subscribe(std::move(handler), priority);
    }

    /// @brief Dispatches an event to all subscribed handlers of type `Event`.
    /// @tparam Event      The event type to process.
    /// @param event       The event object to dispatch.
    template <typename TEvent>
    void ProcessEvent(const TEvent& event)
    {
        GetDispatcher<TEvent>()->ProcessEvent(event);
    }

private:

    class IDispatcherBase
    {
    public:

        virtual ~IDispatcherBase()                   = default;
        virtual bool HasHandlers() const             = 0;
        virtual std::string GetEventTypeName() const = 0;
    };

    template <typename TEvent>
    class Dispatcher;

    template <typename TEvent>
    std::shared_ptr<Dispatcher<TEvent>> GetDispatcher()
    {
        const std::type_index type = typeid(TEvent);

        std::lock_guard lock(m_mutex);
        if (auto it = m_dispatchers.find(type); it != m_dispatchers.end()) {
            return std::static_pointer_cast<Dispatcher<TEvent>>(it->second);
        }

        auto dispatcher     = std::make_shared<Dispatcher<TEvent>>();
        m_dispatchers[type] = dispatcher;
        return dispatcher;
    }

    std::mutex m_mutex;
    std::unordered_map<std::type_index, std::shared_ptr<IDispatcherBase>> m_dispatchers;
};

} // namespace game_engine

namespace game_engine
{

template <typename TEvent>
class EventSystem::Dispatcher final
    : public IDispatcherBase
    , public std::enable_shared_from_this<Dispatcher<TEvent>>
{
public:

    using Handler   = std::function<void(const TEvent&)>;
    using HandlerId = std::size_t;

    struct HandlerNode
    {
        HandlerId id;
        Handler handler;
        HandlerPriority priority;
    };

    class SubscriptionImpl final : public ISubscription
    {
    public:

        SubscriptionImpl(std::weak_ptr<Dispatcher> dispatcher, HandlerId id)
            : m_dispatcher(std::move(dispatcher))
            , m_id(id)
        {}

        ~SubscriptionImpl() override
        {
            Unsubscribe();
        }

        SubscriptionImpl(const SubscriptionImpl&) = delete;
        SubscriptionImpl(SubscriptionImpl&&)      = default;

        SubscriptionImpl& operator=(const SubscriptionImpl&) = delete;
        SubscriptionImpl& operator=(SubscriptionImpl&&)      = default;

        void Unsubscribe() const override
        {
            if (auto dispatcher = m_dispatcher.lock()) {
                dispatcher->Unsubscribe(m_id);
            }
        }

    private:

        std::weak_ptr<Dispatcher> m_dispatcher;
        HandlerId m_id;
    };

    bool HasHandlers() const override
    {
        std::lock_guard lock(m_mutex);
        return !m_handlers.empty();
    }

    std::string GetEventTypeName() const override
    {
        return typeid(TEvent).name();
    }

    SubscriptionPtr Subscribe(Handler&& handler, HandlerPriority priority)
    {
        std::lock_guard lock(m_mutex);
        HandlerId id = m_next_id++;

        std::ignore = AddHandler({id, std::move(handler), priority});
        return std::make_unique<SubscriptionImpl>(this->weak_from_this(), id);
    }

    void Unsubscribe(HandlerId id)
    {
        std::lock_guard lock(m_mutex);
        if (auto map_it = m_handlers_map.find(id); map_it != m_handlers_map.end()) {
            m_handlers.erase(map_it->second);
            m_handlers_map.erase(map_it);
        }
    }

    void ProcessEvent(const TEvent& event) const
    {
        std::list<HandlerNode> handlers_copy;
        {
            std::lock_guard lock(m_mutex);
            handlers_copy = m_handlers;
        }

        for (const auto& node : handlers_copy) {
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

    auto AddHandler(HandlerNode&& node)
    {
        auto it = std::lower_bound(m_handlers.begin(), m_handlers.end(), node, [](const HandlerNode& a, const HandlerNode& b) {
            if (a.priority == b.priority) {
                return a.id < b.id;
            }
            return a.priority > b.priority;
        });

        auto inserted = m_handlers.insert(it, std::move(node));

        try {
            m_handlers_map.emplace(inserted->id, inserted);
        } catch (...) {
            // Rollback: Remove from the list if map insertion failed
            m_handlers.erase(inserted);
            throw;
        }

        return inserted;
    }

    mutable std::mutex m_mutex;
    std::list<HandlerNode> m_handlers;
    std::unordered_map<HandlerId, typename std::list<HandlerNode>::iterator> m_handlers_map;
    HandlerId m_next_id{1};
};

} // namespace game_engine
