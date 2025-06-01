#pragma once

#include <memory>

#include <engine/graphics/texture.hpp>

namespace game_engine
{

class TextureLoader final
{
public:

    std::shared_ptr<ITexture> Load(const TextureLoadParams& params) const;
};

} // namespace game_engine
