#include "resource_manager.hpp"

#include <resource_management/resource_loader.hpp>

namespace game_engine
{

void ResourceManager::RegisterLoader(std::unique_ptr<IResourceLoader> loader)
{
    m_loaders.emplace_back(std::move(loader));
}

bool ResourceManager::IsLoaded(const ResourceId& id) const
{
    auto ptr = GetResource(id);
    return ptr != nullptr && ptr->IsReady();
}

void ResourceManager::Unload(const ResourceId& id)
{}

ResourceManager::ResourcePtr<IResource> ResourceManager::LoadResource(const ResourceId& id, ResourceType type)
{
    for (const auto& loader : m_loaders) {
        if (loader->SupportsType(type)) {
            auto resource = loader->Load(id);
            if (resource && resource->IsReady()) {
                m_cache[id] = resource;
                return resource;
            }
        }
    }

    return nullptr;
}

ResourceManager::ResourcePtr<IResource> ResourceManager::GetResource(const ResourceId& id) const
{
    if (auto it = m_cache.find(id); it != m_cache.end()) {
        return it->second;
    }

    return nullptr;
}

} // namespace game_engine
