#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/mesh.hpp>

namespace game_engine
{

class MeshLoader final
{
public:

    std::shared_ptr<IMesh> Load(ResourceId id, const std::string_view name, const MeshLoadParams& params) const;
};

} // namespace game_engine
