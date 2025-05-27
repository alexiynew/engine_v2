#pragma once

#include <engine/graphics/mesh_data.hpp>
#include <engine/graphics/vertex_traits.hpp>

namespace game_engine::graphics
{

class IMesh
{
public:
    virtual ~IMesh() = default;

    virtual void SetMeshData(const MeshData& data) = 0;
    virtual void Flush()                           = 0;

    virtual void Clear() noexcept         = 0;
    virtual bool IsValid() const noexcept = 0;
};

} // namespace game_engine::graphics
