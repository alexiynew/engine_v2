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

    bool initialize(const GameSettings&) override;
    void shutdown() override;
    void pollEvents() override;
    void beginFrame() override;
    void endFrame() override;

    void applySettings(const GameSettings&) override;

    std::shared_ptr<core::Shader> createShader() override;
    std::shared_ptr<core::Mesh> createMesh() override;

    void render(const std::shared_ptr<core::Mesh>& mesh, const std::shared_ptr<core::Shader>& shader) override;

private:
    int m_framesCount       = 0;
    int m_targetFramesCount = 100;
};

} // namespace game_engine::backend
