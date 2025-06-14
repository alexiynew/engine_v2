#pragma once

#include <vector>

#include <engine/graphics/mesh.hpp>

namespace game_engine
{

class MeshResource final : public IMesh
{
public:

    MeshResource(ResourceId id, const std::string& name);

    // IResource implementation
    bool IsValid() const noexcept override;
    ResourceId GetId() const noexcept override;
    ResourceType GetType() const noexcept override;
    ResourceState GetState() const noexcept override;
    std::string_view GetName() const noexcept override;

    void AddDependency(std::shared_ptr<IResource> resource) override;
    std::vector<std::shared_ptr<IResource>> GetDependencies() const override;

    // IMesh implementation
    void SetVertexData(VertexData data) override;
    void AddSubMesh(SubMesh submesh) override;
    void SetWireframeMode(bool enabled) override;

    const VertexData& GetVertexData() const override;
    const std::vector<SubMesh>& GetSubMeshes() const override;
    bool IsWireframeMode() const override;

    // MeshResource methods
    void SetState(ResourceState state) noexcept;

private:

    const ResourceId m_id = 0;
    std::string m_name;
    ResourceState m_state = ResourceState::NotLoaded;

    VertexData m_vertex_data;
    std::vector<SubMesh> m_submeshes;

    bool m_wireframe = false;
};

} // namespace game_engine
