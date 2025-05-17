#pragma once

#include <engine/resource_id.hpp>

namespace game_engine
{
class ResourceManager;

class ResourceHandler
{
public:
    ResourceHandler(ResourceManager& manager, ResourceId id);

    virtual ~ResourceHandler() = default;

    void load();
    void unload();
    bool isLoaded() const noexcept;
    ResourceId getId() const noexcept;

protected:
    virtual void loadImpl()   = 0;
    virtual void unloadImpl() = 0;

private:
    ResourceManager& m_manager;
    ResourceId m_id;
    bool m_isLoaded = false;
};

} // namespace game_engine
