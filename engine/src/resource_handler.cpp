#include <engine/resource_handler.hpp>

namespace game_engine
{

ResourceHandler::ResourceHandler(ResourceManager& manager, ResourceId id)
    : m_manager(manager)
    , m_id(id)
{}

void ResourceHandler::load()
{
    if (!m_isLoaded) {
        loadImpl();
        m_isLoaded = true;
    }
}

void ResourceHandler::unload()
{
    if (m_isLoaded) {
        unloadImpl();
        m_isLoaded = false;
    }
}

bool ResourceHandler::isLoaded() const noexcept
{
    return m_isLoaded;
}

ResourceId ResourceHandler::getId() const noexcept
{
    return m_id;
}

} // namespace game_engine
