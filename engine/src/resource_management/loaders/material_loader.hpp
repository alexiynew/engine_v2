#pragma once

#include <memory>

#include <engine/graphics/material.hpp>

namespace game_engine
{

class MaterialLoader final
{
public:

    std::shared_ptr<IMaterial> Load(ResourceId id, const std::string_view name, const MaterialLoadParams& params) const;
};

} // namespace game_engine
