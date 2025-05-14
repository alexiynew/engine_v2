#include <engine/engine.hpp>
#include <engine/game.hpp>
#include <engine/module_registrar.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>
#include <modules/render_context.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// TODO: MAke tests work, redo module factory to use dependency injection

class MockRenderer : public game_engine::graphics::Renderer
{
public:
    MOCK_METHOD(bool, initialize, (), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));

    MOCK_METHOD(std::shared_ptr<game_engine::graphics::Mesh>, createMesh, (), (override));
    MOCK_METHOD(std::shared_ptr<game_engine::graphics::Shader>, createShader, (), (override));

    MOCK_METHOD(void, addRenderCommand, (const game_engine::graphics::RenderCommand&), (override));
    MOCK_METHOD(void, executeRenderCommands, (), (override));
    MOCK_METHOD(void, clearRenderCommands, (), (override));
};

class MockBackend : public game_engine::backend::Backend
{
public:
    MOCK_METHOD(bool, initialize, (const game_engine::GameSettings&), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));

    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(std::shared_ptr<const game_engine::RenderContext>, getRenderContext, (), (const, override));

    MOCK_METHOD(void, attachBackendObserver, (game_engine::BackendObserver&), (override));
    MOCK_METHOD(void, detachBackendObserver, (const game_engine::BackendObserver&), (override));
};

class MockGame : public game_engine::Game
{
    MOCK_METHOD(void, onInitialize, (), (override));
    MOCK_METHOD(void, onUpdate, (std::chrono::nanoseconds elapsedTime), (override));
    MOCK_METHOD(void, onDraw, (), (override));
    MOCK_METHOD(void, onShutdown, (), (override));
    MOCK_METHOD(bool, onShouldClose, (), (override));
    MOCK_METHOD(game_engine::GameSettings, getSettings, (), (override));
};

class EngineFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        engine       = game_engine::createEngineInstance();
        mockRenderer = std::make_shared<MockRenderer>();
        mockBackend  = std::make_shared<MockBackend>();
    }

    std::shared_ptr<game_engine::Engine> engine;
    std::shared_ptr<MockRenderer> mockRenderer;
    std::shared_ptr<MockBackend> mockBackend;
};

// Mock factories
namespace game_engine
{

void ModuleRegistrar::RegisterModule<Engine>(ModuleFactory& factory)
{
    factory.registerCreator<Engine>([](const ModuleFactory& mf) { return std::make_shared<EngineImpl>(mf); });
}

template <>
void ModuleFactory<backend::Backend>::RegisterModule()
{
    RegisterCreator([]() { return std::make_shared<MockBackend>(); });
}

template <>
void ModuleFactory<graphics::Renderer>::RegisterModule()
{
    RegisterCreator(
    [](std::shared_ptr<const RenderContext> context) { return std::make_shared<MockRenderer>(std::move(context)); });
}

} // namespace game_engine

TEST_F(EngineFixture, CreateEngineInstance)
{
    EXPECT_NE(engine, nullptr);
}

TEST_F(EngineFixture, TimeManagement)
{
    auto t1 = engine->getTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto t2 = engine->getTime();
    EXPECT_GT(t2 - t1, std::chrono::milliseconds(9));
}

TEST_F(EngineFixture, MeshAndShaderCreation)
{
    EXPECT_CALL(*mockRenderer, createMesh()).WillOnce(testing::Return(nullptr));
    EXPECT_CALL(*mockRenderer, createShader()).WillOnce(testing::Return(nullptr));

    auto mesh   = engine->createMesh();
    auto shader = engine->createShader();
    EXPECT_EQ(mesh, nullptr);
    EXPECT_EQ(shader, nullptr);
}

TEST_F(EngineFixture, EventSystemAccess)
{
    auto& eventSystem = engine->getEventSystem();
    EXPECT_NE(&eventSystem, nullptr);
}

TEST_F(EngineFixture, RenderCommandSubmission)
{
    EXPECT_CALL(*mockRenderer, addRenderCommand(testing::_)).Times(1);

    auto mesh   = engine->createMesh();
    auto shader = engine->createShader();
    engine->render(mesh, shader, {});
}

TEST_F(EngineFixture, MainLoopExecution)
{
    EXPECT_CALL(*mockBackend, initialize(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockRenderer, initialize()).WillOnce(testing::Return(true));
    EXPECT_CALL(*mockBackend, pollEvents()).Times(testing::AtLeast(1));

    std::atomic<bool> stopFlag{false};
    std::thread runner([&] {
        engine->run();
        stopFlag.store(true);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine->setShouldStopFlag();
    runner.join();
    EXPECT_TRUE(stopFlag.load());
}

TEST_F(EngineFixture, WindowCloseEventHandling)
{
    game_engine::WindowCloseEvent event;
    EXPECT_CALL(*mockBackend, detachBackendObserver(testing::_)).Times(1);
    engine->getEventSystem().processEvent(event);
}

TEST_F(EngineFixture, ResourceLeakCheck)
{
    EXPECT_CALL(*mockRenderer, shutdown()).Times(1);
    EXPECT_CALL(*mockBackend, shutdown()).Times(1);

    engine->run();
}
