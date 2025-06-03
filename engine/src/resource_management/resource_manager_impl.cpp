#include "resource_manager_impl.hpp"

#include <resource_management/loaders/material_loader.hpp>
#include <resource_management/loaders/mesh_loader.hpp>
#include <resource_management/loaders/shader_loader.hpp>
#include <resource_management/loaders/texture_loader.hpp>

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
    if (auto r = m_mesh_loader->Load(id, name, params); r != nullptr) {
        m_cache.emplace(id, std::move(r));
    }

    return GetResource<IMesh>(name);
}

std::shared_ptr<IShader> ResourceManagerImpl::LoadShader(const std::string_view name, const ShaderLoadParams& params)
{
    const auto id = GetResourceId(name);
    if (auto r = m_shader_loader->Load(id, name, params); r != nullptr) {
        m_cache.emplace(id, std::move(r));
    }

    return GetResource<IShader>(name);
}

std::shared_ptr<ITexture> ResourceManagerImpl::LoadTexture(const std::string_view name, const TextureLoadParams& params)
{
    const auto id = GetResourceId(name);
    if (auto r = m_texture_loader->Load(id, name, params); r != nullptr) {
        m_cache.emplace(id, std::move(r));
    }

    return GetResource<ITexture>(name);
}

std::shared_ptr<IMaterial> ResourceManagerImpl::LoadMaterial(const std::string_view name, const MaterialLoadParams& params)
{
    const auto id = GetResourceId(name);
    if (auto r = m_material_loader->Load(id, name, params); r != nullptr) {
        m_cache.emplace(id, std::move(r));
    }

    return GetResource<IMaterial>(name);
}

std::shared_ptr<IMesh> ResourceManagerImpl::GetMesh(const std::string_view name) const
{
    return GetResource<IMesh>(name);
}

std::shared_ptr<IShader> ResourceManagerImpl::GetShader(const std::string_view name) const
{
    return GetResource<IShader>(name);
}

std::shared_ptr<ITexture> ResourceManagerImpl::GetTexture(const std::string_view name) const
{
    return GetResource<ITexture>(name);
}

std::shared_ptr<IMaterial> ResourceManagerImpl::GetMaterial(const std::string_view name) const
{
    return GetResource<IMaterial>(name);
}

bool ResourceManagerImpl::IsLoaded(const std::string_view name) const
{
    const auto id = GetResourceId(name);
    if (auto it = m_cache.find(id); it != m_cache.end()) {
        return it->second->IsReady();
    }

    return false;
}

void ResourceManagerImpl::Unload(const std::string_view name)
{
    const auto id = GetResourceId(name);
    if (auto it = m_cache.find(id); it != m_cache.end()) {
        it->second->Unload();
        m_cache.erase(it);
    }
}

void ResourceManagerImpl::UnloadAll()
{
    for (auto [_, r] : m_cache) {
        r->Unload();
    }

    m_cache.clear();
}

#pragma endregion

#pragma region ResourceManagerImpl private

ResourceManagerImpl::ResourceId ResourceManagerImpl::GetResourceId(const std::string_view name) const
{
    // TODO: Solve hash collisions
    return m_hasher(name);
}

template <typename T>
std::shared_ptr<T> ResourceManagerImpl::GetResource(const std::string_view name) const
{
    const auto id = GetResourceId(name);
    if (auto it = m_cache.find(id); it != m_cache.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }

    return nullptr;
}

#pragma endregion

} // namespace game_engine
