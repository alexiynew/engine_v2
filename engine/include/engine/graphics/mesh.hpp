#pragma once

#include <engine/graphics/mesh_data.hpp>
#include <engine/graphics/vertex_traits.hpp>

namespace game_engine::graphics
{

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual void setMeshData(const MeshData& data) = 0;
    virtual void flush()                           = 0;

    /// @brief Clears the mesh by removing it from the context.
    virtual void clear() noexcept = 0;

    /// @brief Checks if the mesh is valid and ready to be used.
    /// @return True if the mesh is valid, false otherwise.
    virtual bool isValid() const noexcept = 0;
};

} // namespace game_engine::graphics
