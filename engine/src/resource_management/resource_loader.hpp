#pragma once

#include <memory>

#include <resource_management/resource.hpp>

namespace game_engine
{

class IResourceLoader
{
public:
    virtual ~IResourceLoader() = default;

    virtual std::shared_ptr<IResource> Load(const ResourceId& id) const;
    virtual bool SupportsType(ResourceType type) const;
};

} // namespace game_engine
