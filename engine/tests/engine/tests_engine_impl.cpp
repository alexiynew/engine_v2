#include <engine/game.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>
#include <modules/module_locator.hpp>

#include <engine_impl.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockRenderer : public game_engine::graphics::IRenderer
{
public:
    MOCK_METHOD(bool, init, (std::shared_ptr<const game_engine::IRenderContext>), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));

    MOCK_METHOD(std::shared_ptr<game_engine::graphics::IMesh>, createMesh, (), (override));
    MOCK_METHOD(std::shared_ptr<game_engine::graphics::IShader>, createShader, (), (override));

    MOCK_METHOD(void, addRenderCommand, (const game_engine::graphics::RenderCommand&), (override));
    MOCK_METHOD(void, executeRenderCommands, (), (override));
    MOCK_METHOD(void, clearRenderCommands, (), (override));
};

class MockBackend : public game_engine::backend::IBackend
{
public:
    MOCK_METHOD(bool, init, (const game_engine::GameSettings&), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));

    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(std::shared_ptr<const game_engine::IRenderContext>, getRenderContext, (), (const, override));

    MOCK_METHOD(void, attachBackendObserver, (game_engine::IBackendObserver&), (override));
    MOCK_METHOD(void, detachBackendObserver, (const game_engine::IBackendObserver&), (override));
};

class MockGame : public game_engine::IGame
{
public:
    MOCK_METHOD(bool, init, (std::shared_ptr<game_engine::IEngine>), (noexcept, override));
    MOCK_METHOD(void, shutdown, (), (noexcept, override));

    MOCK_METHOD(void, onUpdate, (std::chrono::nanoseconds elapsedTime), (override));
    MOCK_METHOD(void, onDraw, (), (override));
    MOCK_METHOD(bool, onShouldClose, (), (override));
    MOCK_METHOD(game_engine::GameSettings, getSettings, (), (override));
};

class EngineFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto create_module_locator = [&]() {
            game_engine::ModuleLocator ml;

            ml.setImplementation<game_engine::backend::IBackend>(m_mock_backend);
            ml.setImplementation<game_engine::graphics::IRenderer>(m_mock_renderer);
            ml.setImplementation<game_engine::IGame>(m_mock_game);

            return ml;
        };

        m_mock_backend  = std::make_shared<::testing::NiceMock<MockBackend>>();
        m_mock_renderer = std::make_shared<::testing::NiceMock<MockRenderer>>();
        m_mock_game     = std::make_shared<::testing::NiceMock<MockGame>>();

        m_engine = std::make_shared<game_engine::EngineImpl>(create_module_locator());
    }

    std::shared_ptr<MockBackend> m_mock_backend;
    std::shared_ptr<MockRenderer> m_mock_renderer;
    std::shared_ptr<MockGame> m_mock_game;

    game_engine::ModuleLocator m_locator;

    std::shared_ptr<game_engine::EngineImpl> m_engine;
};

TEST_F(EngineFixture, CreateEngine)
{
    EXPECT_NE(m_engine, nullptr);
}

TEST_F(EngineFixture, TimeManagement)
{
    auto t1 = m_engine->getTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto t2 = m_engine->getTime();
    EXPECT_GT(t2 - t1, std::chrono::milliseconds(9));
}

TEST_F(EngineFixture, MeshAndShaderCreation)
{
    using namespace testing;

    EXPECT_CALL(*m_mock_renderer, createMesh()).WillOnce(Return(nullptr));
    EXPECT_CALL(*m_mock_renderer, createShader()).WillOnce(Return(nullptr));

    auto mesh   = m_engine->createMesh();
    auto shader = m_engine->createShader();
    EXPECT_EQ(mesh, nullptr);
    EXPECT_EQ(shader, nullptr);
}

TEST_F(EngineFixture, EventSystemAccess)
{
    auto& event_system = m_engine->getEventSystem();
    EXPECT_NE(&event_system, nullptr);
}

TEST_F(EngineFixture, RenderCommandSubmission)
{
    using namespace testing;

    EXPECT_CALL(*m_mock_renderer, createMesh()).WillOnce(Return(nullptr));
    EXPECT_CALL(*m_mock_renderer, createShader()).WillOnce(Return(nullptr));
    EXPECT_CALL(*m_mock_renderer, addRenderCommand(_)).Times(1);

    auto mesh   = m_engine->createMesh();
    auto shader = m_engine->createShader();
    m_engine->render(mesh, shader, {});
}

TEST_F(EngineFixture, MainLoopExecution)
{
    using namespace testing;

    {
        Sequence seq1;
        EXPECT_CALL(*m_mock_backend, init(_)).WillOnce(Return(true));
        EXPECT_CALL(*m_mock_renderer, init(_)).WillOnce(Return(true));
        EXPECT_CALL(*m_mock_game, init(_)).WillOnce(Return(true));
    }

    EXPECT_CALL(*m_mock_backend, pollEvents()).Times(testing::AtLeast(1));

    {
        Sequence seq2;
        EXPECT_CALL(*m_mock_game, shutdown()).Times(1);
        EXPECT_CALL(*m_mock_renderer, shutdown()).Times(1);
        EXPECT_CALL(*m_mock_backend, shutdown()).Times(1);
    }

    // Free modules in fixture to prevent resource leak errors
    m_mock_backend.reset();
    m_mock_renderer.reset();
    m_mock_game.reset();

    std::atomic<bool> stop_flag{false};
    std::thread runner([&] {
        m_engine->run();
        stop_flag.store(true);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m_engine->setShouldStopFlag();
    runner.join();

    EXPECT_TRUE(stop_flag.load());
}
