#include "material_loader.hpp"

#include <resource_management/resources/material_resource.hpp>

namespace game_engine
{

std::shared_ptr<MaterialResource> MaterialLoader::Load(ResourceId id, std::string_view name, const MaterialLoadParams& params) const
{
    return nullptr;
}

} // namespace game_engine
