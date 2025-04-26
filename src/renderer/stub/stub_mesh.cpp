#include "stub_mesh.hpp"

namespace game_engine::renderer
{

void StubMesh::setMeshData(const core::MeshData&)
{}

void StubMesh::flush()
{}

void StubMesh::clear() noexcept
{}

bool StubMesh::isValid() const noexcept
{
    return true;
}

} // namespace game_engine::renderer
