#pragma once

#include <memory>
#include <string>
#include <vector>

namespace game_engine
{

enum class ResourceState
{
    NotLoaded,
    LoadedInRAM,
    LoadedInGPU,
    Error,
};

enum class ResourceType
{
    Unknown,
    Mesh,
    Shader,
    Texture,
    Material,
};

using ResourceId = std::size_t;

class IResource
{
public:

    virtual ~IResource() = default;

    virtual bool IsValid() const noexcept             = 0;
    virtual ResourceId GetId() const noexcept         = 0;
    virtual ResourceType GetType() const noexcept     = 0;
    virtual ResourceState GetState() const noexcept   = 0;
    virtual std::string_view GetName() const noexcept = 0;

    virtual void AddDependency(std::shared_ptr<IResource> resource)         = 0;
    virtual std::vector<std::shared_ptr<IResource>> GetDependencies() const = 0;
};

} // namespace game_engine
