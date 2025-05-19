#pragma once

#include <functional>
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
        virtual void unsubscribe() const = 0;
    };

    using SubscriptionPtr = std::unique_ptr<ISubscription>;

    EventSystem() = default;
    ~EventSystem();

    /// @brief Subscribes a handler to events of type `Event`.
    /// @tparam Event      The event type to subscribe to.
    /// @param handler     Callback function to invoke when the event is dispatched.
    /// @param priority    Priority of the handler (default: HandlerPriority::Whenever).
    /// @returns           A subscription handle for unsubscribing.
    template <typename Event>
    [[nodiscard]]
    SubscriptionPtr subscribe(std::function<void(const Event&)> handler,
                              HandlerPriority priority = HandlerPriority::Whenever)
    {
        return getDispatcher<Event>()->subscribe(std::move(handler), priority);
    }

    /// @brief Dispatches an event to all subscribed handlers of type `Event`.
    /// @tparam Event      The event type to process.
    /// @param event       The event object to dispatch.
    template <typename Event>
    void processEvent(const Event& event)
    {
        getDispatcher<Event>()->processEvent(event);
    }

private:
    class IDispatcherBase
    {
    public:
        virtual ~IDispatcherBase()                   = default;
        virtual bool hasHandlers() const             = 0;
        virtual std::string getEventTypeName() const = 0;
    };

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
    std::unordered_map<std::type_index, std::shared_ptr<IDispatcherBase>> m_dispatchers;
};

} // namespace game_engine

#include <engine/details/event_system_details.inl>
