#include "stub_mesh.hpp"

namespace game_engine::graphics
{

void StubMesh::setMeshData(const graphics::MeshData&)
{}

void StubMesh::flush()
{}

void StubMesh::clear() noexcept
{}

bool StubMesh::isValid() const noexcept
{
    return true;
}

} // namespace game_engine::graphics
