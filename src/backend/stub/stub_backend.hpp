#pragma once

#include <memory>

#include "../backend.hpp"

namespace game_engine::backend
{

class StubBackend final : public Backend
{
public:
    StubBackend();
    ~StubBackend() override;

    bool initialize() override;
    void shutdown() override;
    void pollEvents() override;
    void beginFrame() override;
    void endFrame() override;

    core::MeshId loadMesh(const core::Mesh& mesh) override;
    void renderMesh(core::MeshId meshId) override;

private:
    int m_framesCount       = 0;
    int m_targetFramesCount = 100;
};

} // namespace game_engine::backend
