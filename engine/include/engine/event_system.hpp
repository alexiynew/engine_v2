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
    SubscriptionPtr Subscribe(std::function<void(const TEvent&)> handler,
                              HandlerPriority priority = HandlerPriority::Whenever)
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

#include <engine/details/event_system_details.inl>
