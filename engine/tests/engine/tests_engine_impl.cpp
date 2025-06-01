#include <engine/game.hpp>

#include <modules/backend/backend.hpp>
#include <modules/graphics/renderer.hpp>
#include <modules/module_locator.hpp>

#include <engine_impl.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockRenderer : public game_engine::graphics::IRendererModule
{
public:
    MOCK_METHOD(bool, Init, (std::shared_ptr<game_engine::graphics::IRenderContext>), (noexcept, override));
    MOCK_METHOD(void, Shutdown, (), (noexcept, override));

    // MOCK_METHOD(std::shared_ptr<game_engine::IMesh>, CreateMesh, (), (override));
    // MOCK_METHOD(std::shared_ptr<game_engine::IShader>, CreateShader, (), (override));

    // MOCK_METHOD(void, AddRenderCommand, (const game_engine::graphics::RenderCommand&), (override));
    // MOCK_METHOD(void, ExecuteRenderCommands, (), (override));
    // MOCK_METHOD(void, ClearRenderCommands, (), (override));
};

class MockBackend : public game_engine::backend::IBackendModule
{
public:
    MOCK_METHOD(bool, Init, (const game_engine::GameSettings&), (noexcept, override));
    MOCK_METHOD(void, Shutdown, (), (noexcept, override));

    MOCK_METHOD(void, AttachBackendObserver, (game_engine::IBackendObserver&), (override));
    MOCK_METHOD(void, DetachBackendObserver, (const game_engine::IBackendObserver&), (override));

    MOCK_METHOD(void, PollEvents, (), (const, override));

    MOCK_METHOD(void, MakeContextCurrent, (), (const, override));
    MOCK_METHOD(void, DropCurrentContext, (), (const, override));
    MOCK_METHOD(void, SwapBuffers, (), (const, override));
};

class MockGame : public game_engine::IGame
{
public:
    MOCK_METHOD(bool, Init, (std::shared_ptr<game_engine::IEngine>), (noexcept, override));
    MOCK_METHOD(void, Shutdown, (), (noexcept, override));

    MOCK_METHOD(void, OnUpdate, (std::chrono::nanoseconds elapsedTime), (override));
    MOCK_METHOD(void, OnDraw, (), (override));
    MOCK_METHOD(bool, OnShouldClose, (), (override));
    MOCK_METHOD(game_engine::GameSettings, GetSettings, (), (override));
};

class EngineFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto create_module_locator = [&]() {
            game_engine::ModuleLocator ml;

            ml.SetImplementation<game_engine::backend::IBackendModule>(m_mock_backend);
            ml.SetImplementation<game_engine::graphics::IRendererModule>(m_mock_renderer);
            ml.SetImplementation<game_engine::IGame>(m_mock_game);

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
    auto t1 = m_engine->GetTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto t2 = m_engine->GetTime();
    EXPECT_GT(t2 - t1, std::chrono::milliseconds(9));
}

TEST_F(EngineFixture, MeshAndShaderCreation)
{
    using namespace testing;

    // EXPECT_CALL(*m_mock_renderer, CreateMesh()).WillOnce(Return(nullptr));
    // EXPECT_CALL(*m_mock_renderer, CreateShader()).WillOnce(Return(nullptr));

    // auto mesh   = m_engine->CreateMesh();
    // auto shader = m_engine->CreateShader();
    // EXPECT_EQ(mesh, nullptr);
    // EXPECT_EQ(shader, nullptr);
}

TEST_F(EngineFixture, EventSystemAccess)
{
    auto event_system = m_engine->GetEventSystem();
    EXPECT_NE(event_system, nullptr);
}

TEST_F(EngineFixture, RenderCommandSubmission)
{
    using namespace testing;

    // EXPECT_CALL(*m_mock_renderer, CreateMesh()).WillOnce(Return(nullptr));
    // EXPECT_CALL(*m_mock_renderer, CreateShader()).WillOnce(Return(nullptr));
    // EXPECT_CALL(*m_mock_renderer, AddRenderCommand(_)).Times(1);

    // auto mesh   = m_engine->CreateMesh();
    // auto shader = m_engine->CreateShader();
    // m_engine->Render(mesh, shader, {});
}

TEST_F(EngineFixture, MainLoopExecution)
{
    using namespace testing;

    {
        Sequence seq1;
        EXPECT_CALL(*m_mock_backend, Init(_)).WillOnce(Return(true));
        EXPECT_CALL(*m_mock_renderer, Init(_)).WillOnce(Return(true));
        EXPECT_CALL(*m_mock_game, Init(_)).WillOnce(Return(true));
    }

    EXPECT_CALL(*m_mock_backend, PollEvents()).Times(testing::AtLeast(1));

    {
        Sequence seq2;
        EXPECT_CALL(*m_mock_game, Shutdown()).Times(1);
        EXPECT_CALL(*m_mock_renderer, Shutdown()).Times(1);
        EXPECT_CALL(*m_mock_backend, Shutdown()).Times(1);
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

    m_engine->SetShouldStopFlag();
    runner.join();

    EXPECT_TRUE(stop_flag.load());
}
