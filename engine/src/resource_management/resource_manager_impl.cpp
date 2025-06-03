#include "resource_manager_impl.hpp"

#include <resource_management/loaders/material_loader.hpp>
#include <resource_management/loaders/mesh_loader.hpp>
#include <resource_management/loaders/shader_loader.hpp>
#include <resource_management/loaders/texture_loader.hpp>
#include <resource_management/resources/material_resource.hpp>
#include <resource_management/resources/mesh_resource.hpp>
#include <resource_management/resources/shader_resource.hpp>
#include <resource_management/resources/texture_resource.hpp>

namespace
{

template <typename T>
using ResourceMap = std::unordered_map<game_engine::ResourceId, std::shared_ptr<T>>;

template <typename T>
inline std::shared_ptr<T> GetResource(game_engine::ResourceId id, const ResourceMap<T>& container)
{
    if (auto it = container.find(id); it != container.end()) {
        return it->second;
    }

    return nullptr;
}

template <typename T, typename TLoaderType, typename TLoadParams>
inline std::shared_ptr<T> LoadResource(game_engine::ResourceId id,
    const std::string_view name,
    const TLoaderType& loader,
    const TLoadParams& params,
    ResourceMap<T>& container)
{
    if (container.find(id) != container.end()) {
        return nullptr; // Resource already loaded
    }

    if (auto r = loader->Load(id, name, params); r != nullptr) {
        r->SetState(game_engine::ResourceState::LoadedInRAM);
        container.emplace(id, r);
        return r;
    }
    return nullptr;
}

} // namespace

namespace game_engine
{

ResourceManagerImpl::ResourceManagerImpl()
    : m_mesh_loader(std::make_unique<MeshLoader>())
    , m_shader_loader(std::make_unique<ShaderLoader>())
    , m_texture_loader(std::make_unique<TextureLoader>())
    , m_material_loader(std::make_unique<MaterialLoader>())
{}

ResourceManagerImpl::~ResourceManagerImpl()
{
    UnloadAll();
}

#pragma region IResourceManager

std::shared_ptr<IMesh> ResourceManagerImpl::LoadMesh(const std::string_view name, const MeshLoadParams& params)
{
    const auto id = GetResourceId(name);
    return LoadResource<MeshResource>(id, name, m_mesh_loader, params, m_meshes);
}

std::shared_ptr<IShader> ResourceManagerImpl::LoadShader(const std::string_view name, const ShaderLoadParams& params)
{
    const auto id = GetResourceId(name);
    return LoadResource<ShaderResource>(id, name, m_shader_loader, params, m_shaders);
}

std::shared_ptr<ITexture> ResourceManagerImpl::LoadTexture(const std::string_view name, const TextureLoadParams& params)
{
    const auto id = GetResourceId(name);
    return LoadResource<TextureResource>(id, name, m_texture_loader, params, m_textures);
}

std::shared_ptr<IMaterial> ResourceManagerImpl::LoadMaterial(const std::string_view name, const MaterialLoadParams& params)
{
    const auto id = GetResourceId(name);
    return LoadResource<MaterialResource>(id, name, m_material_loader, params, m_materials);
}

std::shared_ptr<IMesh> ResourceManagerImpl::GetMesh(const std::string_view name) const
{
    return GetResource<MeshResource>(GetResourceId(name), m_meshes);
}

std::shared_ptr<IShader> ResourceManagerImpl::GetShader(const std::string_view name) const
{
    return GetResource<ShaderResource>(GetResourceId(name), m_shaders);
}

std::shared_ptr<ITexture> ResourceManagerImpl::GetTexture(const std::string_view name) const
{
    return GetResource<TextureResource>(GetResourceId(name), m_textures);
}

std::shared_ptr<IMaterial> ResourceManagerImpl::GetMaterial(const std::string_view name) const
{
    return GetResource<MaterialResource>(GetResourceId(name), m_materials);
}

bool ResourceManagerImpl::IsLoaded(const std::string_view name) const
{
    const auto id = GetResourceId(name);
    return m_meshes.find(id) != m_meshes.end() || m_shaders.find(id) != m_shaders.end() || m_textures.find(id) != m_textures.end() ||
           m_materials.find(id) != m_materials.end();
}

void ResourceManagerImpl::Unload(const std::string_view name)
{
    const auto id = GetResourceId(name);

    if (auto it = m_meshes.find(id); it != m_meshes.end()) {
        it->second->SetState(ResourceState::NotLoaded);
        m_meshes.erase(it);
        return;
    }

    if (auto it = m_shaders.find(id); it != m_shaders.end()) {
        it->second->SetState(ResourceState::NotLoaded);
        m_shaders.erase(it);
        return;
    }

    if (auto it = m_textures.find(id); it != m_textures.end()) {
        it->second->SetState(ResourceState::NotLoaded);
        m_textures.erase(it);
        return;
    }

    if (auto it = m_materials.find(id); it != m_materials.end()) {
        it->second->SetState(ResourceState::NotLoaded);
        m_materials.erase(it);
        return;
    }
}

void ResourceManagerImpl::UnloadAll()
{
    for (auto& [id, resource] : m_meshes) {
        resource->SetState(ResourceState::NotLoaded);
    }
    m_meshes.clear();

    for (auto& [id, resource] : m_shaders) {
        resource->SetState(ResourceState::NotLoaded);
    }
    m_shaders.clear();

    for (auto& [id, resource] : m_textures) {
        resource->SetState(ResourceState::NotLoaded);
    }
    m_textures.clear();

    for (auto& [id, resource] : m_materials) {
        resource->SetState(ResourceState::NotLoaded);
    }
    m_materials.clear();
}

#pragma endregion

#pragma region ResourceManagerImpl private

ResourceManagerImpl::ResourceId ResourceManagerImpl::GetResourceId(const std::string_view name) const
{
    return m_hasher(name);
}

#pragma endregion

} // namespace game_engine
