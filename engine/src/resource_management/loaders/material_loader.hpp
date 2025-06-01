#pragma once

#include <memory>

#include <engine/graphics/material.hpp>

namespace game_engine
{

class MaterialLoader final
{
public:

    std::shared_ptr<IMaterial> Load(const MaterialLoadParams& params) const;
};

} // namespace game_engine
