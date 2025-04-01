#pragma once

#include <game_engine/core/mesh.hpp>

namespace game_engine::backend
{

class StubMesh final : public core::Mesh
{
public:

    // core::mesh
    void setMeshData(const core::MeshData& data) override;
    void flush() override;
    void clear() override;
    bool isValid() const override;
    void render() const override;
};

} // namespace game_engine::backend
