#include <engine/resource_id.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace game_engine;

TEST(ResourceId, DefaultConstruction)
{
    ResourceId id;
    EXPECT_EQ(id.type, ResourceId::Type::Unknown);
    EXPECT_EQ(id.id, 0u);

    EXPECT_EQ(id, ResourceId::InvalidId);
}

TEST(ResourceId, TypeAndIdConstruction)
{
    ResourceId id(ResourceId::Type::Texture, 123u);
    EXPECT_EQ(id.type, ResourceId::Type::Texture);
    EXPECT_EQ(id.id, 123u);
}

TEST(ResourceId, StringHashConstruction)
{
    static constexpr auto expected_hash = details::FNV1aHash("test");
    ResourceId id(ResourceId::Type::Shader, "test");
    EXPECT_EQ(id.id, expected_hash);
}

TEST(ResourceId, EqualityAndOrdering)
{
    ResourceId a(ResourceId::Type::Texture, 100u);
    ResourceId b(ResourceId::Type::Texture, 100u);
    ResourceId c(ResourceId::Type::Shader, 100u);
    ResourceId d(ResourceId::Type::Texture, 200u);

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
    EXPECT_LT(a, d); // Check operator <=>
}

TEST(ResourceId, HashFunction)
{
    ResourceId id(ResourceId::Type::Mesh, 456u);
    const std::hash<ResourceId> hasher;
    const std::size_t hash = hasher(id);

    ResourceId other_id(ResourceId::Type::Mesh, 457u);
    const std::size_t other_hash = hasher(other_id);
    EXPECT_NE(hash, other_hash);
}

TEST(ResourceId, InvalidId)
{
    EXPECT_EQ(ResourceId::InvalidId.type, ResourceId::Type::Unknown);
    EXPECT_EQ(ResourceId::InvalidId.id, 0u);
}

TEST(Fnv1aHash, BasicHashing)
{
    static constexpr auto hash1 = details::FNV1aHash("hello");
    static constexpr auto hash2 = details::FNV1aHash("world");
    static constexpr auto hash3 = details::FNV1aHash("hello");

    EXPECT_NE(hash1, hash2);
    EXPECT_EQ(hash1, hash3);
}
