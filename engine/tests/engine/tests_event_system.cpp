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

TEST_F(EventSystemFixture, SubscriptionDifferentEvents)
{
    struct Event0
    {};

    struct Event1
    {};

    struct Event2
    {};

    struct Event3
    {};

    struct Event4
    {};

    struct Event5
    {};

    struct Event6
    {};

    struct Event7
    {};

    struct Event8
    {};

    struct Event9
    {};

    int handledValue0 = 0;
    int handledValue1 = 0;
    int handledValue2 = 0;
    int handledValue3 = 0;
    int handledValue4 = 0;
    int handledValue5 = 0;
    int handledValue6 = 0;
    int handledValue7 = 0;
    int handledValue8 = 0;
    int handledValue9 = 0;

    auto sub0 = es.subscribe<Event0>([&](const auto& event) { handledValue0++; });
    auto sub1 = es.subscribe<Event1>([&](const auto& event) { handledValue1++; });
    auto sub2 = es.subscribe<Event2>([&](const auto& event) { handledValue2++; });
    auto sub3 = es.subscribe<Event3>([&](const auto& event) { handledValue3++; });
    auto sub4 = es.subscribe<Event4>([&](const auto& event) { handledValue4++; });
    auto sub5 = es.subscribe<Event5>([&](const auto& event) { handledValue5++; });
    auto sub6 = es.subscribe<Event6>([&](const auto& event) { handledValue6++; });
    auto sub7 = es.subscribe<Event7>([&](const auto& event) { handledValue7++; });
    auto sub8 = es.subscribe<Event8>([&](const auto& event) { handledValue8++; });
    auto sub9 = es.subscribe<Event9>([&](const auto& event) { handledValue9++; });

    es.processEvent(Event0{});
    es.processEvent(Event1{});
    es.processEvent(Event2{});
    es.processEvent(Event3{});
    es.processEvent(Event4{});
    es.processEvent(Event5{});
    es.processEvent(Event6{});
    es.processEvent(Event7{});
    es.processEvent(Event8{});
    es.processEvent(Event9{});

    EXPECT_EQ(handledValue0, 1);
    EXPECT_EQ(handledValue1, 1);
    EXPECT_EQ(handledValue2, 1);
    EXPECT_EQ(handledValue3, 1);
    EXPECT_EQ(handledValue4, 1);
    EXPECT_EQ(handledValue5, 1);
    EXPECT_EQ(handledValue6, 1);
    EXPECT_EQ(handledValue7, 1);
    EXPECT_EQ(handledValue8, 1);
    EXPECT_EQ(handledValue9, 1);
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

TEST_F(EventSystemFixture, MultithreadedSubscription)
{
    constexpr int Threads         = 10;
    constexpr int EventsPerThread = 1000;

    std::atomic<int> counter{0};
    std::atomic<bool> running{true};
    std::vector<std::thread> threads;

    for (int i = 0; i < Threads; ++i) {
        threads.emplace_back([&] {
            auto sub = es.subscribe<int>([&](const auto&) { counter++; });
            while (running) {
                // Wait for processing
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    // Wait for threads to start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    for (int j = 0; j < EventsPerThread; ++j) {
        es.processEvent(0);
    }

    // Stop threads
    running = false;
    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(counter, Threads * EventsPerThread);
}
