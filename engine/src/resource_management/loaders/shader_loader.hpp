#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/shader.hpp>

namespace game_engine
{

class ShaderLoader final
{
public:

    std::shared_ptr<IShader> Load(ResourceId id, const std::string_view name, const ShaderLoadParams& params) const;
};

} // namespace game_engine
