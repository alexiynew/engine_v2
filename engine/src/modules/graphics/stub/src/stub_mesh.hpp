#pragma once

#include <engine/graphics/mesh.hpp>

namespace game_engine::graphics
{

class StubMesh final : public graphics::IMesh
{
public:

    // graphics::mesh
    void SetMeshData(const graphics::MeshData& data) override;
    void Flush() override;
    void Clear() noexcept override;
    bool IsValid() const noexcept override;
};

} // namespace game_engine::graphics
