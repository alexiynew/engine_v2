#pragma once

#include <string_view>
#include <type_traits>

#include "engine/common_types.hpp"

namespace game_engine::helpers
{

constexpr std::uint32_t FNV1aHash(std::string_view str) noexcept
{
    constexpr uint32_t Prime = 16777619U;

    std::uint32_t hash = 2166136261U;
    for (const char c : str) {
        hash = (hash ^ static_cast<uint8_t>(c)) * Prime;
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

} // namespace game_engine::helpers
