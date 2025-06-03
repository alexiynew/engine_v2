#pragma once

#include <memory>
#include <string_view>

#include <engine/graphics/texture.hpp>

namespace game_engine
{

class TextureLoader final
{
public:

    std::shared_ptr<ITexture> Load(ResourceId id, std::string_view name, const TextureLoadParams& params) const;
};

} // namespace game_engine
