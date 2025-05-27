#include "resource_id.hpp"

#include <helpers/hash_helpers.hpp>

namespace game_engine
{

ResourceId::ResourceId(const std::string_view str)
    : m_hash(helpers::FNV1aHash(str))
{}

} // namespace game_engine

namespace std
{

std::size_t hash<game_engine::ResourceId>::operator()(const game_engine::ResourceId& rid) const noexcept
{
    return rid.m_hash;
}

} // namespace std
