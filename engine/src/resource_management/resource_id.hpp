#pragma once

#include <cstdint>
#include <string_view>

namespace game_engine
{

class ResourceId
{
public:
    explicit ResourceId(const std::string_view str);

    friend constexpr bool operator==(const ResourceId&, const ResourceId&)  = default;
    friend constexpr auto operator<=>(const ResourceId&, const ResourceId&) = default;

    friend struct std::hash<ResourceId>;

private:
    std::uint32_t m_hash;
};

} // namespace game_engine

// The std::hash specialization
namespace std
{

template <>
struct hash<game_engine::ResourceId>
{
    std::size_t operator()(const game_engine::ResourceId& rid) const noexcept;
};

} // namespace std
