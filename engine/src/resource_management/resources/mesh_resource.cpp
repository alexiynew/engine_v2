#include "mesh_resource.hpp"

namespace game_engine
{

MeshResource::MeshResource(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool MeshResource::IsValid() const noexcept
{
    bool is_valid = true;

    is_valid &= m_vertex_data.vertex_count != 0;
    is_valid &= m_vertex_data.vertex_size != 0;
    is_valid &= !m_vertex_data.data.empty();
    is_valid &= !m_vertex_data.attributes.empty();
    is_valid &= !m_submeshes.empty();
    is_valid &= m_state != ResourceState::Error;

    return is_valid;
}

ResourceId MeshResource::GetId() const noexcept
{
    return m_id;
}

ResourceType MeshResource::GetType() const noexcept
{
    return ResourceType::Mesh;
}

ResourceState MeshResource::GetState() const noexcept
{
    return m_state;
}

std::string_view MeshResource::GetName() const noexcept
{
    return m_name;
}

void MeshResource::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> MeshResource::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region IMesh implementation

void MeshResource::SetVertexData(VertexData data)
{
    m_vertex_data = std::move(data);
}

void MeshResource::AddSubMesh(SubMesh submesh)
{
    m_submeshes.push_back(std::move(submesh));
}

void MeshResource::SetPrimitiveType(PrimitiveType type)
{
    m_primitive_type = type;
}

const VertexData& MeshResource::GetVertexData() const
{
    return m_vertex_data;
}

const std::vector<SubMesh>& MeshResource::GetSubMeshes() const
{
    return m_submeshes;
}

PrimitiveType MeshResource::GetPrimitiveType() const
{
    return m_primitive_type;
}

#pragma endregion

#pragma region MeshResource methods

void MeshResource::SetState(ResourceState state) noexcept
{
    m_state = state;
}

#pragma endregion

} // namespace game_engine
