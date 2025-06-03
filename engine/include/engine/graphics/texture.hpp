#pragma once

#include <filesystem>

#include <engine/resource.hpp>

namespace game_engine
{

enum class TextureType
{
    Diffuse,
    Normal,
    Specular,
    Environment,
    RenderTarget
};

enum class TextureWrapMode
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

struct TextureLoadParams
{
    std::filesystem::path source;
    TextureType type          = TextureType::Diffuse;
    bool generate_mipmaps     = true;
    TextureWrapMode wrap_mode = TextureWrapMode::Repeat;
};

class ITexture : public IResource
{
public:

    ~ITexture() override = default;
};

} // namespace game_engine
