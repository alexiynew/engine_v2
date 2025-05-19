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

    /// @brief Clears the mesh by removing it from the context.
    virtual void Clear() noexcept = 0;

    /// @brief Checks if the mesh is valid and ready to be used.
    /// @return True if the mesh is valid, false otherwise.
    virtual bool IsValid() const noexcept = 0;
};

} // namespace game_engine::graphics
