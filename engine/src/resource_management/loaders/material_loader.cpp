#include "material_loader.hpp"

namespace game_engine
{

std::shared_ptr<IMaterial> MaterialLoader::Load(ResourceId id, const std::string_view name, const MaterialLoadParams& params) const
{
    return nullptr;
}

} // namespace game_engine
