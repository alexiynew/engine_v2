#pragma once

#include <backend/backend.hpp>

namespace game_engine::backend
{

class StubBackend final
    : public Backend
    , public renderer::RendererContext
    , public std::enable_shared_from_this<StubBackend>
{
public:
    StubBackend();
    ~StubBackend() override;

    // Backend
    bool initialize(const GameSettings&) override;
    void shutdown() override;
    void pollEvents() override;

    std::shared_ptr<renderer::RendererContext> getRendererContext() override;

    // renderer::RendererContext
    void makeCurrent() override;
    void dropCurrent() override;
    void swapBuffers() override;

private:
    int m_framesCount       = 0;
    int m_targetFramesCount = 100;
};

} // namespace game_engine::backend
