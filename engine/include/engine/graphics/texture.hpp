#pragma once

#include <cstdint>

namespace game_engine::graphics
{

class ITexture
{
public:
    virtual ~ITexture() = default;

    virtual void Bind(std::uint32_t slot = 0) const = 0;
    virtual void Unbind() const                     = 0;

    virtual void Clear() noexcept         = 0;
    virtual bool IsValid() const noexcept = 0;
};

} // namespace game_engine::graphics
