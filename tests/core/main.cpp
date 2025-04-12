#include <backend/backend.hpp>
#include <core/engine_impl.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace game_engine;
using namespace ::testing;

class ShaderMock : public game_engine::core::Shader
{
public:
    ShaderMock()
    {
        ON_CALL(*this, link()).WillByDefault(Return(true));
        ON_CALL(*this, isValid()).WillByDefault(Return(true));
    }

    MOCK_METHOD(void, setSource, (const std::string&, const std::string&), (override));
    MOCK_METHOD(bool, link, (), (override));
    MOCK_METHOD(void, setUniform, (const std::string&, const core::Uniform&), (override));
    MOCK_METHOD(void, clear, (), (noexcept, override));
    MOCK_METHOD(bool, isValid, (), (const, noexcept, override));
};

class GameMock final : public game_engine::Game
{
public:
    MOCK_METHOD(void, onInitialize, (), (override));
    MOCK_METHOD(void, onUpdate, (std::chrono::nanoseconds elapsedTime), (override));
    MOCK_METHOD(void, onDraw, (), (override));
    MOCK_METHOD(void, onShutdown, (), (override));
    MOCK_METHOD(void, onKeyboardInputEvent, (const KeyboardInputEvent& event), (override));
    MOCK_METHOD(bool, onShouldClose, (), (override));
    MOCK_METHOD(GameSettings, getSettings, (), (override));
};

class BackendMock final : public game_engine::backend::Backend
{
public:
    explicit BackendMock(int requiredFramesCount)
        : m_requiredFramesCount(requiredFramesCount)
    {}

    MOCK_METHOD(bool, initialize, (const GameSettings&), (override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(void, beginFrame, (), (override));
    MOCK_METHOD(void, endFrame, (), (override));
    MOCK_METHOD(void, applySettings, (const GameSettings&), (override));
    MOCK_METHOD(std::shared_ptr<core::Shader>, createShader, (), (override));
    MOCK_METHOD(std::shared_ptr<core::Mesh>, createMesh, (), (override));
    MOCK_METHOD(void, render, (const std::shared_ptr<core::Mesh>&, const std::shared_ptr<core::Shader>&), (override));

    void testPollEvents()
    {
        if (m_frameCount >= m_requiredFramesCount) {
            notify(WindowCloseEvent{});
        }
    }

    void testEndFrame()
    {
        m_frameCount++;
    }

private:
    int m_requiredFramesCount = 0;
    int m_frameCount          = 0;
};

TEST(CoreTest, DefaultMainLoop)
{
    constexpr int maxFramesCount = 10;

    auto backend = std::make_shared<BackendMock>(maxFramesCount);
    auto engine  = std::make_shared<game_engine::core::EngineImpl>(backend);

    // Test backend calls
    EXPECT_CALL(*backend, initialize(GameSettings{})).WillOnce(Return(true));

    EXPECT_CALL(*backend, pollEvents()).Times(AtLeast(maxFramesCount)).WillRepeatedly(Invoke([&backend]() {
        backend->testPollEvents();
    }));
    EXPECT_CALL(*backend, beginFrame()).Times(maxFramesCount);
    EXPECT_CALL(*backend, endFrame()).Times(maxFramesCount).WillRepeatedly(Invoke([&backend]() {
        backend->testEndFrame();
    }));

    EXPECT_CALL(*backend, createShader())
    .Times(AnyNumber())
    .WillRepeatedly(Return(std::make_shared<NiceMock<ShaderMock>>()));

    EXPECT_CALL(*backend, shutdown());

    // Test game calls
    auto game = std::make_shared<GameMock>();
    engine->setGameInstance(game);

    EXPECT_CALL(*game, onInitialize).Times(1);
    EXPECT_CALL(*game, onUpdate).Times(AnyNumber());
    EXPECT_CALL(*game, onDraw).Times(AnyNumber());
    EXPECT_CALL(*game, onShouldClose).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*game, onShutdown).Times(1);
    EXPECT_CALL(*game, getSettings).Times(1).WillOnce(Return(GameSettings{}));

    const int returnCode = engine->run();

    EXPECT_EQ(0, returnCode);
}

int main(int argc, char** argv)
{
    InitGoogleTest(&argc, argv);
    InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
