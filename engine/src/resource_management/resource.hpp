#pragma once

#include <resource_management/resource_id.hpp>

namespace game_engine
{

enum class ResourceType : std::uint32_t
{
    Unknown,
    Texture,
    Shader,
    Mesh
};

class IResource
{
public:
    virtual ~IResource() = default;

    virtual bool IsReady() const noexcept         = 0;
    virtual ResourceType GetType() const noexcept = 0;
    virtual ResourceId GetId() const noexcept     = 0;
};

} // namespace game_engine
