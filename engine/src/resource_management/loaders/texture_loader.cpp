#include "texture_loader.hpp"

#include <resource_management/resources/texture_resource.hpp>

namespace game_engine
{

std::shared_ptr<TextureResource> TextureLoader::Load(ResourceId id, std::string_view name, const TextureLoadParams& params) const
{
    return nullptr;
}

} // namespace game_engine
