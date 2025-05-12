#include <engine/event_system.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class EventSystemFixture : public ::testing::Test
{
protected:
    game_engine::EventSystem es;
};

TEST_F(EventSystemFixture, BasicSubscription)
{
    struct Event
    {
        int value = 0;
    };

    int handledValue = 0;

    auto sub = es.subscribe<Event>([&](const Event& event) { handledValue = event.value; });
    es.processEvent(Event{42});

    EXPECT_EQ(handledValue, 42);
}

TEST_F(EventSystemFixture, HandlerPriorityOrder)
{
    using game_engine::HandlerPriority;

    std::vector<int> executionOrder;

    auto s1 = es.subscribe<int>([&](const auto&) { executionOrder.push_back(0); }, HandlerPriority::Whenever);
    auto s2 = es.subscribe<int>([&](const auto&) { executionOrder.push_back(1); }, HandlerPriority::UrgentButCanVibe);
    auto s3 = es.subscribe<int>([&](const auto&) { executionOrder.push_back(2); }, HandlerPriority::RedPanic);

    es.processEvent(0);

    ASSERT_EQ(executionOrder.size(), 3);
    EXPECT_EQ(executionOrder[0], 2);
    EXPECT_EQ(executionOrder[1], 1);
    EXPECT_EQ(executionOrder[2], 0);
}

TEST_F(EventSystemFixture, ManualUnsubscription)
{
    int counter = 0;

    auto sub = es.subscribe<int>([&](const auto&) { counter++; });
    sub->unsubscribe();

    es.processEvent(0);

    EXPECT_EQ(counter, 0);
}

TEST_F(EventSystemFixture, SelfUnsubscriptionInHandler)
{
    int counter = 0;
    game_engine::EventSystem::SubscriptionPtr selfSub;

    selfSub = es.subscribe<int>([&](const auto&) {
        counter++;
        selfSub->unsubscribe();
    });

    es.processEvent(0); // Must be called once
    es.processEvent(0); // No effect

    EXPECT_EQ(counter, 1);
}

TEST_F(EventSystemFixture, MultithreadedProcessing)
{
    constexpr int Threads         = 4;
    constexpr int EventsPerThread = 1000;

    std::atomic<int> counter{0};
    std::vector<std::thread> threads;

    auto sub = es.subscribe<int>([&](const auto&) { counter++; });

    for (int i = 0; i < Threads; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < EventsPerThread; ++j) {
                es.processEvent(42);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(counter, Threads * EventsPerThread);
}

TEST_F(EventSystemFixture, ThreadLocalHandlers)
{
    thread_local int tlsCounter = 0;

    auto sub = es.subscribe<int>([&](const auto&) { tlsCounter++; });

    std::thread([&] { es.processEvent(0); }).join();

    EXPECT_EQ(tlsCounter, 0); // TLS must not change in main thread
}
