#pragma once

#include <string_view>

#include "engine/common_types.hpp"

namespace game_engine::details
{

inline constexpr std::uint32_t FNV1aHash(std::string_view str) noexcept
{
    constexpr uint32_t prime = 16777619u;

    std::uint32_t hash = 2166136261u;
    for (const char c : str) {
        hash = (hash ^ static_cast<uint8_t>(c)) * prime;
    }

    return hash;
}

template <typename T>
requires std::is_integral_v<T>
constexpr T HashCombine(T seed, T value) noexcept
{
    auto hasher = std::hash<T>{};
    return seed ^ hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace game_engine::details
