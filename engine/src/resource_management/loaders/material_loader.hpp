#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/material.hpp>

namespace game_engine
{

class MaterialResource;

class MaterialLoader final
{
public:

    std::shared_ptr<MaterialResource> Load(ResourceId id, const std::string_view name, const MaterialLoadParams& params) const;
};

} // namespace game_engine
