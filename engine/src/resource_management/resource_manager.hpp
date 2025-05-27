#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <resource_management/resource.hpp>

namespace game_engine
{

namespace graphics
{

class IMesh;
class IShader;
class ITexture;

}; // namespace graphics

class IResourceLoader;

template <typename T>
constexpr ResourceType GetStaticResourceType()
{
    if constexpr (std::is_base_of_v<graphics::IMesh, T>) {
        return ResourceType::Mesh;
    } else if constexpr (std::is_base_of_v<graphics::IShader, T>) {
        return ResourceType::Shader;
    } else if constexpr (std::is_base_of_v<graphics::ITexture, T>) {
        return ResourceType::Texture;
    }

    return ResourceType::Unknown;
}

class ResourceManager final
{
public:
    template <typename T>
    using ResourcePtr = std::shared_ptr<T>;

    void RegisterLoader(std::unique_ptr<IResourceLoader> loader);

    template <typename T>
    ResourcePtr<T> Load(const ResourceId& id)
    {
        if (auto ptr = GetResource(id); ptr != nullptr) {
            return std::dynamic_pointer_cast<T>(ptr);
        }

        constexpr ResourceType type = GetStaticResourceType<T>();
        return std::dynamic_pointer_cast<T>(LoadResource(id, type));
    }

    template <typename T>
    ResourcePtr<T> Get(const ResourceId& id) const
    {
        return std::dynamic_pointer_cast<T>(GetResource(id));
    }

    bool IsLoaded(const ResourceId& id) const;

    void Unload(const ResourceId& id);

private:
    ResourcePtr<IResource> LoadResource(const ResourceId& id, ResourceType type);
    ResourcePtr<IResource> GetResource(const ResourceId& id) const;

    std::vector<std::unique_ptr<IResourceLoader>> m_loaders;
    std::unordered_map<ResourceId, ResourcePtr<IResource>> m_cache;
};

} // namespace game_engine
