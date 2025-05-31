#pragma once

#include <memory>

#include <engine/graphics/shader.hpp>

namespace game_engine
{

class ShaderLoader final
{
public:
    std::shared_ptr<IShader> Load(const ShaderLoadParams& params) const;
};

} // namespace game_engine
