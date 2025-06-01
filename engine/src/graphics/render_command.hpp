#pragma once

#include <memory>
#include <vector>

#include <engine/graphics/mesh.hpp>
#include <engine/graphics/property.hpp>
#include <engine/graphics/shader.hpp>

namespace game_engine
{

struct RenderCommand
{
    std::shared_ptr<IMesh> mesh;
    std::shared_ptr<IShader> shader;
    std::vector<Property> properties;
    std::uint32_t instance_count = 1;
};

} // namespace game_engine
