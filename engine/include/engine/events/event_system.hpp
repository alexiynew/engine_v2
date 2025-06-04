#pragma once

#include <functional>
#include <memory>
#include <typeindex>

namespace game_engine
{

/// @brief Priority levels for event handlers. Higher priority handlers execute first.
enum class HandlerPriority : std::uint8_t
{
    Whenever         = 0,
    UrgentButCanVibe = 1,
    RedPanic         = 2,
};

class ISubscription
{
public:

    virtual ~ISubscription() = default;

    virtual void Unsubscribe() noexcept = 0;
};

class IEventSystem
{
public:

    using SubscriptionPtr = std::unique_ptr<ISubscription>;
    template <typename T>
    using EventHandler = std::function<void(const T&)>;

    virtual ~IEventSystem() = default;

    template <typename TEvent>
    [[nodiscard]]
    SubscriptionPtr Subscribe(EventHandler<TEvent> handler, HandlerPriority priority = HandlerPriority::Whenever)
    {
        return SubscribeImpl(typeid(TEvent), [h = std::move(handler)](const void* event) {
            h(*static_cast<const TEvent*>(event));
        }, priority);
    }

    template <typename TEvent>
    void ProcessEvent(const TEvent& event)
    {
        ProcessEventImpl(typeid(TEvent), &event);
    }

protected:

    using GenericHandler = std::function<void(const void*)>;

    virtual SubscriptionPtr SubscribeImpl(std::type_index event_type, GenericHandler handler, HandlerPriority priority) = 0;
    virtual void ProcessEventImpl(std::type_index event_type, const void* event)                                        = 0;
};

} // namespace game_engine
