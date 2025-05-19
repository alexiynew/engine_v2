#pragma once

#include <engine/graphics/mesh.hpp>

namespace game_engine::graphics
{

class StubMesh final : public graphics::IMesh
{
public:

    // graphics::mesh
    void setMeshData(const graphics::MeshData& data) override;
    void flush() override;
    void clear() noexcept override;
    bool isValid() const noexcept override;
};

} // namespace game_engine::graphics
