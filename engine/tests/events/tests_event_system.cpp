#include <engine/events/event_system.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class EventSystemFixture : public ::testing::Test
{
protected:

    game_engine::EventSystem m_es;
};

TEST_F(EventSystemFixture, BasicSubscription)
{
    struct Event
    {
        int value = 0;
    };

    int handled_value = 0;

    auto sub = m_es.Subscribe<Event>([&](const Event& event) { handled_value = event.value; });
    m_es.ProcessEvent(Event{42});

    EXPECT_EQ(handled_value, 42);
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

    int handled_value0 = 0;
    int handled_value1 = 0;
    int handled_value2 = 0;
    int handled_value3 = 0;
    int handled_value4 = 0;
    int handled_value5 = 0;
    int handled_value6 = 0;
    int handled_value7 = 0;
    int handled_value8 = 0;
    int handled_value9 = 0;

    auto sub0 = m_es.Subscribe<Event0>([&](const auto& event) { handled_value0++; });
    auto sub1 = m_es.Subscribe<Event1>([&](const auto& event) { handled_value1++; });
    auto sub2 = m_es.Subscribe<Event2>([&](const auto& event) { handled_value2++; });
    auto sub3 = m_es.Subscribe<Event3>([&](const auto& event) { handled_value3++; });
    auto sub4 = m_es.Subscribe<Event4>([&](const auto& event) { handled_value4++; });
    auto sub5 = m_es.Subscribe<Event5>([&](const auto& event) { handled_value5++; });
    auto sub6 = m_es.Subscribe<Event6>([&](const auto& event) { handled_value6++; });
    auto sub7 = m_es.Subscribe<Event7>([&](const auto& event) { handled_value7++; });
    auto sub8 = m_es.Subscribe<Event8>([&](const auto& event) { handled_value8++; });
    auto sub9 = m_es.Subscribe<Event9>([&](const auto& event) { handled_value9++; });

    m_es.ProcessEvent(Event0{});
    m_es.ProcessEvent(Event1{});
    m_es.ProcessEvent(Event2{});
    m_es.ProcessEvent(Event3{});
    m_es.ProcessEvent(Event4{});
    m_es.ProcessEvent(Event5{});
    m_es.ProcessEvent(Event6{});
    m_es.ProcessEvent(Event7{});
    m_es.ProcessEvent(Event8{});
    m_es.ProcessEvent(Event9{});

    EXPECT_EQ(handled_value0, 1);
    EXPECT_EQ(handled_value1, 1);
    EXPECT_EQ(handled_value2, 1);
    EXPECT_EQ(handled_value3, 1);
    EXPECT_EQ(handled_value4, 1);
    EXPECT_EQ(handled_value5, 1);
    EXPECT_EQ(handled_value6, 1);
    EXPECT_EQ(handled_value7, 1);
    EXPECT_EQ(handled_value8, 1);
    EXPECT_EQ(handled_value9, 1);
}

TEST_F(EventSystemFixture, HandlerPriorityOrder)
{
    using game_engine::HandlerPriority;

    std::vector<int> execution_order;

    auto s1 = m_es.Subscribe<int>([&](const auto&) { execution_order.push_back(0); }, HandlerPriority::Whenever);
    auto s2 = m_es.Subscribe<int>([&](const auto&) { execution_order.push_back(1); }, HandlerPriority::UrgentButCanVibe);
    auto s3 = m_es.Subscribe<int>([&](const auto&) { execution_order.push_back(2); }, HandlerPriority::RedPanic);

    m_es.ProcessEvent(0);

    ASSERT_EQ(execution_order.size(), 3);
    EXPECT_EQ(execution_order[0], 2);
    EXPECT_EQ(execution_order[1], 1);
    EXPECT_EQ(execution_order[2], 0);
}

TEST_F(EventSystemFixture, ManualUnsubscription)
{
    int counter = 0;

    auto sub = m_es.Subscribe<int>([&](const auto&) { counter++; });
    sub->Unsubscribe();

    m_es.ProcessEvent(0);

    EXPECT_EQ(counter, 0);
}

TEST_F(EventSystemFixture, SelfUnsubscriptionInHandler)
{
    int counter = 0;
    game_engine::EventSystem::SubscriptionPtr self_sub;

    self_sub = m_es.Subscribe<int>([&](const auto&) {
        counter++;
        self_sub->Unsubscribe();
    });

    m_es.ProcessEvent(0); // Must be called once
    m_es.ProcessEvent(0); // No effect

    EXPECT_EQ(counter, 1);
}

TEST_F(EventSystemFixture, MultithreadedProcessing)
{
    constexpr int ThreadsCount    = 4;
    constexpr int EventsPerThread = 1000;

    std::atomic<int> counter{0};
    std::vector<std::thread> threads;

    auto sub = m_es.Subscribe<int>([&](const auto&) { counter++; });

    for (int i = 0; i < ThreadsCount; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < EventsPerThread; ++j) {
                m_es.ProcessEvent(42);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(counter, ThreadsCount * EventsPerThread);
}

TEST_F(EventSystemFixture, MultithreadedSubscription)
{
    constexpr int ThreadsCount    = 10;
    constexpr int EventsPerThread = 1000;

    std::atomic<int> counter{0};
    std::atomic<bool> running{true};
    std::vector<std::thread> threads;

    for (int i = 0; i < ThreadsCount; ++i) {
        threads.emplace_back([&] {
            auto sub = m_es.Subscribe<int>([&](const auto&) { counter++; });
            while (running) {
                // Wait for processing
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    // Wait for threads to start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    for (int j = 0; j < EventsPerThread; ++j) {
        m_es.ProcessEvent(0);
    }

    // Stop threads
    running = false;
    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(counter, ThreadsCount * EventsPerThread);
}
