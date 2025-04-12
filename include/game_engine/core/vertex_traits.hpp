#pragma once

#include <bit>
#include <cstring>

#include <game_engine/core/mesh_data.hpp>

namespace game_engine::core
{

namespace vertex_traits
{
template <typename T>
inline constexpr VertexAttributeType getAttributeType() noexcept
{
    if constexpr (std::is_same_v<T, float>)
        return VertexAttributeType::Float;
    else if constexpr (std::is_same_v<T, int>)
        return VertexAttributeType::Int;
    else if constexpr (std::is_same_v<T, unsigned int>)
        return VertexAttributeType::UInt;
    else
        static_assert(!std::is_same_v<T, T>, "Unsupported vertex attribute type");
}

template <typename T>
inline constexpr int getComponentCount() noexcept
{
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, unsigned int>)
        return 1; // for scalar types
    else if constexpr (std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector2u>)
        return 2;
    else if constexpr (std::is_same_v<T, Vector3> || std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector3u>)
        return 3;
    else if constexpr (std::is_same_v<T, Vector4> || std::is_same_v<T, Vector4i> || std::is_same_v<T, Vector4u>)
        return 4;
    else
        static_assert(!std::is_same_v<T, T>, "Unsupported vertex attribute type");
}

template <typename T, typename U>
inline std::size_t memberOffset(U T::* ptr) noexcept
{
    constexpr T* nullObj = nullptr;
    return std::bit_cast<std::size_t>(&(nullObj->*ptr));
}

template <typename T>
concept SupportedVertexAttribute = std::is_same_v<T, Vector2> || std::is_same_v<T, Vector3> ||
                                   std::is_same_v<T, Vector4> || std::is_same_v<T, Vector2i> ||
                                   std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector4i> ||
                                   std::is_same_v<T, Vector2u> || std::is_same_v<T, Vector3u> ||
                                   std::is_same_v<T, Vector4u>;

template <typename AttributeType>
struct ScalarTypeHelper
{
    static_assert(SupportedVertexAttribute<AttributeType>, "Unsupported type");
    using Type = decltype([] {
        if constexpr (std::is_same_v<AttributeType, Vector2> || std::is_same_v<AttributeType, Vector3> ||
                      std::is_same_v<AttributeType, Vector4>)
        {
            return float{};
        } else if constexpr (std::is_same_v<AttributeType, Vector2i> || std::is_same_v<AttributeType, Vector3i> ||
                             std::is_same_v<AttributeType, Vector4i>)
        {
            return int{};
        } else if constexpr (std::is_same_v<AttributeType, Vector2u> || std::is_same_v<AttributeType, Vector3i> ||
                             std::is_same_v<AttributeType, Vector4i>)
        {
            return (unsigned int){};
        } else {
            return (void*){};
        }
    }());
};

template <typename AttributeType>
using ScalarType = typename ScalarTypeHelper<AttributeType>::Type;

} // namespace vertex_traits

template <typename VertexType, typename MemberType>
inline constexpr VertexAttribute generateAttribute(int location,
                                                   const char* name,
                                                   MemberType(VertexType::* ptr),
                                                   bool normalized = false) noexcept
{
    static_assert(vertex_traits::SupportedVertexAttribute<MemberType>, "Unsupported vertex attribute type");

    const std::size_t offset = vertex_traits::memberOffset(ptr);

    using ScalarType = vertex_traits::ScalarType<MemberType>;

    return {location,
            vertex_traits::getComponentCount<MemberType>(),
            offset,
            vertex_traits::getAttributeType<ScalarType>(),
            normalized,
            name};
}

template <typename AttributeType>
inline constexpr VertexAttribute generateAttribute(int location, const char* name, bool normalized = false) noexcept
{
    using ScalarType = vertex_traits::ScalarType<AttributeType>;

    return {location,
            vertex_traits::getComponentCount<AttributeType>(),
            0,
            vertex_traits::getAttributeType<ScalarType>(),
            normalized,
            name};
}

template <typename VertexType>
MeshData createMeshData(const std::vector<VertexType>& vertices,
                        const std::vector<SubMesh>& submeshes,
                        PrimitiveType primitiveType,
                        VertexLayout&& layout)
{
    static_assert(std::is_standard_layout_v<VertexType>, "Vertex type must be standard layout");

    MeshData data = {.vertexData    = {},
                     .vertexCount   = vertices.size(),
                     .submeshes     = submeshes,
                     .instances     = {},
                     .primitiveType = primitiveType,
                     .layout        = std::move(layout)};

    data.vertexData.resize(vertices.size() * sizeof(VertexType));
    std::memcpy(data.vertexData.data(), vertices.data(), data.vertexData.size());

    return data;
}

} // namespace game_engine::core
