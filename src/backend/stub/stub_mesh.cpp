#include "stub_mesh.hpp"

namespace game_engine::backend
{

void StubMesh::setMeshData(const core::MeshData&)
{}

void StubMesh::flush()
{}

void StubMesh::clear()
{}

bool StubMesh::isValid() const
{
    return true;
}

void StubMesh::render() const
{}

} // namespace game_engine::backend
