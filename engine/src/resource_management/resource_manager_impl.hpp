#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <engine/resource_management/resource_manager.hpp>

namespace game_engine
{

class MeshResource;
class ShaderResource;
class TextureResource;
class MaterialResource;

class MeshLoader;
class ShaderLoader;
class TextureLoader;
class MaterialLoader;

class ResourceManagerImpl final : public IResourceManager
{
public:

    using ResourceId = std::size_t;

    ResourceManagerImpl();
    ~ResourceManagerImpl() override;

    ResourceManagerImpl(const ResourceManagerImpl&) = delete;
    ResourceManagerImpl(ResourceManagerImpl&&)      = default;

    ResourceManagerImpl& operator=(const ResourceManagerImpl&) = delete;
    ResourceManagerImpl& operator=(ResourceManagerImpl&&)      = default;

    // IResourceManager implementation
    std::shared_ptr<IMesh> LoadMesh(std::string_view name, const MeshLoadParams& params) override;
    std::shared_ptr<IShader> LoadShader(std::string_view name, const ShaderLoadParams& params) override;
    std::shared_ptr<ITexture> LoadTexture(std::string_view name, const TextureLoadParams& params) override;
    std::shared_ptr<IMaterial> LoadMaterial(std::string_view name, const MaterialLoadParams& params) override;

    std::shared_ptr<IMesh> CreateEmptyMesh(std::string_view name) override;
    std::shared_ptr<IShader> CreateEmptyShader(std::string_view name) override;
    std::shared_ptr<ITexture> CreateEmptyTexture(std::string_view name) override;
    std::shared_ptr<IMaterial> CreateEmptyMaterial(std::string_view name) override;

    std::shared_ptr<IMesh> GetMesh(std::string_view name) const override;
    std::shared_ptr<IShader> GetShader(std::string_view name) const override;
    std::shared_ptr<ITexture> GetTexture(std::string_view name) const override;
    std::shared_ptr<IMaterial> GetMaterial(std::string_view name) const override;

    bool IsLoaded(std::string_view name) const override;
    void Unload(std::string_view name) override;
    void UnloadAll() override;

private:

    ResourceId GetResourceId(std::string_view name) const;

    std::hash<std::string_view> m_hasher;

    std::unique_ptr<MeshLoader> m_mesh_loader;
    std::unique_ptr<ShaderLoader> m_shader_loader;
    std::unique_ptr<TextureLoader> m_texture_loader;
    std::unique_ptr<MaterialLoader> m_material_loader;

    std::unordered_map<ResourceId, std::shared_ptr<MeshResource>> m_meshes;
    std::unordered_map<ResourceId, std::shared_ptr<ShaderResource>> m_shaders;
    std::unordered_map<ResourceId, std::shared_ptr<TextureResource>> m_textures;
    std::unordered_map<ResourceId, std::shared_ptr<MaterialResource>> m_materials;
};

} // namespace game_engine
