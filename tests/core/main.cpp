#include <chrono>
#include <thread>

#include <backend/backend.hpp>
#include <core/engine_impl.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <renderer/renderer.hpp>

using namespace game_engine;
using namespace ::testing;

class BackendMock final : public backend::Backend
{
public:
    MOCK_METHOD(bool, initialize, (const GameSettings&), (override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(std::shared_ptr<renderer::RendererContext>, getRendererContext, (), (override));
};

class RendererMock final : public renderer::Renderer
{
public:
    MOCK_METHOD(bool, initialize, (), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));
    MOCK_METHOD(std::shared_ptr<core::Shader>, createShader, (), (noexcept, override));
    MOCK_METHOD(std::shared_ptr<core::Mesh>, createMesh, (), (noexcept, override));
    MOCK_METHOD(void, addRenderCommand, (const renderer::RenderCommand&), (override));
    MOCK_METHOD(void, clearRenderCommands, (), (noexcept, override));
    MOCK_METHOD(void, executeRenderCommands, (), (noexcept, override));
};

class GameMock final : public Game
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

TEST(CoreTest, DefaultMainLoop)
{
    auto backend  = std::make_shared<BackendMock>();
    auto renderer = std::make_shared<RendererMock>();
    auto engine   = std::make_shared<game_engine::core::EngineImpl>(backend, renderer);

    {
        InSequence seq;
        EXPECT_CALL(*backend, initialize(GameSettings{})).WillOnce(Return(true));
        EXPECT_CALL(*renderer, initialize()).WillOnce(Return(true));

        EXPECT_CALL(*backend, pollEvents()).Times(AnyNumber());

        EXPECT_CALL(*renderer, shutdown());
        EXPECT_CALL(*backend, shutdown());
    }

    EXPECT_CALL(*renderer, clearRenderCommands()).Times(AnyNumber());
    EXPECT_CALL(*renderer, executeRenderCommands()).Times(AnyNumber());

    // Test game calls
    auto game = std::make_shared<GameMock>();
    engine->setGameInstance(game);

    EXPECT_CALL(*game, onInitialize).Times(1);
    EXPECT_CALL(*game, onUpdate).Times(AnyNumber());
    EXPECT_CALL(*game, onDraw).Times(AnyNumber());
    EXPECT_CALL(*game, onShouldClose).Times(AnyNumber());
    EXPECT_CALL(*game, onShutdown).Times(1);
    EXPECT_CALL(*game, getSettings).Times(1).WillOnce(Return(GameSettings{}));

    std::thread t([engine]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        engine->setShouldStopFlag();
    });

    const int returnCode = engine->run();

    t.join();

    EXPECT_EQ(0, returnCode);
}

int main(int argc, char** argv)
{
    InitGoogleTest(&argc, argv);
    InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
