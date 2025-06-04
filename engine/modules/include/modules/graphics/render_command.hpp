#pragma once

#include <memory>
#include <vector>

#include <engine/graphics/property.hpp>
#include <engine/resource_management/resource.hpp>

namespace game_engine::graphics
{

struct BeginFrameCommand
{};

struct EndFrameCommand
{};

struct RenderCommand
{
    ResourceId mesh;
    ResourceId shader;
    std::vector<Property> properties;
    std::uint32_t instance_count = 1;
};

} // namespace game_engine::graphics
