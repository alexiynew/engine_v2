#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/shader.hpp>

namespace game_engine
{

class ShaderResource;

class ShaderLoader final
{
public:

    std::shared_ptr<ShaderResource> Load(ResourceId id, std::string_view name, const ShaderLoadParams& params) const;
};

} // namespace game_engine
