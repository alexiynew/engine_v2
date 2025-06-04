#pragma once

#include <string>
#include <variant>

#include <engine/common_types.hpp>
#include <engine/graphics/texture.hpp>

namespace game_engine
{

/// @brief Type alias for a variant that can hold various types of uniform values.
using PropertyValue = std::variant<int, float, Vector2, Vector3, Vector4, Matrix3, Matrix4, std::shared_ptr<ITexture>>;

struct Property
{
    std::string name;
    PropertyValue value;
};

} // namespace game_engine
