#include <backend/backend.hpp>
#include <core/engine_impl.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace game_engine;
using ::testing::AtLeast;
using ::testing::DoDefault;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::WithArg;

// TODO: Use proper Game mock
class GameMock
{};

class BackendMock final : public game_engine::backend::Backend
{
public:
    explicit BackendMock(int requiredFramesCount)
        : m_requiredFramesCount(requiredFramesCount)
    {}

    MOCK_METHOD(bool, initialize, (), (override));
    MOCK_METHOD(void, shutdown, (), (override));
    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(void, beginFrame, (), (override));
    MOCK_METHOD(void, endFrame, (), (override));
    MOCK_METHOD(core::MeshId, loadMesh, (const core::Mesh&), (override));
    MOCK_METHOD(void, renderMesh, (core::MeshId meshId), (override));

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

    EXPECT_CALL(*backend, initialize()).WillOnce(Return(true));

    EXPECT_CALL(*backend, pollEvents()).Times(AtLeast(maxFramesCount)).WillRepeatedly(Invoke([&backend]() {
        backend->testPollEvents();
    }));
    EXPECT_CALL(*backend, beginFrame()).Times(maxFramesCount);
    EXPECT_CALL(*backend, endFrame()).Times(maxFramesCount).WillRepeatedly(Invoke([&backend]() {
        backend->testEndFrame();
    }));

    EXPECT_CALL(*backend, shutdown());

    //    auto game   = std::make_shared<GameMock>(engine);

    //    engine->setGame(game);

    const int returnCode = engine->run();

    EXPECT_EQ(0, returnCode);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
