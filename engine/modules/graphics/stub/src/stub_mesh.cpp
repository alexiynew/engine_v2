#include "stub_mesh.hpp"

namespace game_engine::graphics
{

void StubMesh::SetMeshData(const graphics::MeshData&)
{}

void StubMesh::Flush()
{}

void StubMesh::Clear() noexcept
{}

bool StubMesh::IsValid() const noexcept
{
    return true;
}

} // namespace game_engine::graphics
