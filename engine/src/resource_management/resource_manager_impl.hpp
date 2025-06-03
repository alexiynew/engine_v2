#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <engine/resource_manager.hpp>

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

    // IResourceManager
    std::shared_ptr<IMesh> LoadMesh(const std::string_view name, const MeshLoadParams& params) override;
    std::shared_ptr<IShader> LoadShader(const std::string_view name, const ShaderLoadParams& params) override;
    std::shared_ptr<ITexture> LoadTexture(const std::string_view name, const TextureLoadParams& params) override;
    std::shared_ptr<IMaterial> LoadMaterial(const std::string_view name, const MaterialLoadParams& params) override;

    std::shared_ptr<IMesh> GetMesh(const std::string_view name) const override;
    std::shared_ptr<IShader> GetShader(const std::string_view name) const override;
    std::shared_ptr<ITexture> GetTexture(const std::string_view name) const override;
    std::shared_ptr<IMaterial> GetMaterial(const std::string_view name) const override;

    bool IsLoaded(const std::string_view name) const override;
    void Unload(const std::string_view name) override;
    void UnloadAll() override;

private:

    ResourceId GetResourceId(const std::string_view name) const;

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
