#pragma once

#include <cstdint>
#include <string_view>

#include <engine/details/hash_helpers.hpp>

namespace game_engine
{

struct ResourceId final
{
    enum class Type : std::uint32_t
    {
        Unknown,
        Texture,
        Shader,
        Mesh,
    };

    constexpr ResourceId() = default;

    constexpr ResourceId(Type t, std::uint32_t i)
        : type(t)
        , id(i)
    {}

    constexpr ResourceId(Type t, const std::string_view str)
        : type(t)
        , id(details::FNV1aHash(str))
    {}

    friend constexpr bool operator==(const ResourceId&, const ResourceId&)  = default;
    friend constexpr auto operator<=>(const ResourceId&, const ResourceId&) = default;

    static const ResourceId InvalidId;

    Type type        = Type::Unknown;
    std::uint32_t id = 0;
};

inline const ResourceId ResourceId::InvalidId{};

} // namespace game_engine

namespace std
{

template <>
struct hash<game_engine::ResourceId>
{
    std::size_t operator()(const game_engine::ResourceId& rid) const noexcept
    {
        std::size_t seed = 0;

        seed = game_engine::details::HashCombine(seed, static_cast<std::size_t>(rid.type));
        seed = game_engine::details::HashCombine(seed, static_cast<std::size_t>(rid.id));
        return seed;
    }
};

} // namespace std

