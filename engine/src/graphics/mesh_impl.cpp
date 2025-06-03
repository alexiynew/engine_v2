#include "mesh_impl.hpp"

namespace game_engine
{

MeshImpl::MeshImpl(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool MeshImpl::Load()
{
    bool is_data_ready = true;

    is_data_ready &= m_vertex_data.vertex_count != 0;
    is_data_ready &= m_vertex_data.vertex_size != 0;
    is_data_ready &= !m_vertex_data.data.empty();
    is_data_ready &= !m_vertex_data.attributes.empty();
    is_data_ready &= !m_submeshes.empty();
    is_data_ready &= m_state != ResourceState::Error;

    if (is_data_ready) {
        m_state = ResourceState::LoadedInRAM;
    }

    return is_data_ready;
}

bool MeshImpl::Unload()
{
    // UnloadFormGPU();

    m_vertex_data = {};
    m_submeshes.clear();

    m_state = ResourceState::NotLoaded;

    return true;
}

bool MeshImpl::Reload()
{
    if (m_state == ResourceState::LoadedInGPU) {
        m_state = ResourceState::Loading;
    }

    return IsReady();
}

bool MeshImpl::IsReady() const noexcept
{
    return m_state == ResourceState::LoadedInGPU;
}

ResourceId MeshImpl::GetId() const noexcept
{
    return m_id;
}

std::string MeshImpl::GetName() const noexcept
{
    return m_name;
}

ResourceType MeshImpl::GetType() const noexcept
{
    return ResourceType::Mesh;
}

ResourceState MeshImpl::GetState() const noexcept
{
    return m_state;
}

void MeshImpl::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> MeshImpl::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region IMesh implementation

void MeshImpl::SetVertexData(const VertexData& data)
{
    m_vertex_data = data;
}

void MeshImpl::AddSubMesh(const SubMesh& submesh)
{
    m_submeshes.push_back(submesh);
}

void MeshImpl::SetPrimitiveType(PrimitiveType type)
{
    m_primitive_type = type;
}

const VertexData& MeshImpl::GetVertexData() const
{
    return m_vertex_data;
}

const std::vector<SubMesh>& MeshImpl::GetSubMeshes() const
{
    return m_submeshes;
}

PrimitiveType MeshImpl::GetPrimitiveType() const
{
    return m_primitive_type;
}

#pragma endregion

} // namespace game_engine
