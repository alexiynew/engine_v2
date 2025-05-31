#pragma once

#include <memory>
#include <string>
#include <vector>

namespace game_engine
{

enum class ResourceState
{
    NotLoaded,
    Loading,
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

class IResource
{
public:
    virtual ~IResource() = default;

    virtual bool Load()   = 0;
    virtual bool Unload() = 0;
    virtual bool Reload() = 0;

    virtual ResourceState GetState() const noexcept = 0;
    virtual ResourceType GetType() const noexcept   = 0;
    virtual std::string GetName() const noexcept    = 0;
    virtual bool IsReady() const noexcept           = 0;

    virtual void AddDependency(std::shared_ptr<IResource> resource)         = 0;
    virtual std::vector<std::shared_ptr<IResource>> GetDependencies() const = 0;
};

} // namespace game_engine
