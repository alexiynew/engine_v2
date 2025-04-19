#pragma once

#include <bit>
#include <cstring>
#include <type_traits>

#include <game_engine/core/mesh_data.hpp>

namespace game_engine::core
{

namespace vertex_traits
{

template <typename T>
inline constexpr bool IsFloatVertexAttribute = std::is_same_v<T, float> || std::is_same_v<T, Vector2> ||
                                               std::is_same_v<T, Vector3> || std::is_same_v<T, Vector4>;

template <typename T>
inline constexpr bool IsIntVertexAttribute = std::is_same_v<T, int> || std::is_same_v<T, Vector2i> ||
                                             std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector4i>;

template <typename T>
inline constexpr bool IsUIntVertexAttribute = std::is_same_v<T, unsigned int> || std::is_same_v<T, Vector2u> ||
                                              std::is_same_v<T, Vector3u> || std::is_same_v<T, Vector4u>;

template <typename T>
inline constexpr bool SupportedVertexAttribute = IsFloatVertexAttribute<T> || IsIntVertexAttribute<T> ||
                                                 IsUIntVertexAttribute<T>;

template <typename T>
inline constexpr VertexAttributeType getAttributeType() noexcept
{
    if constexpr (IsFloatVertexAttribute<T>)
        return VertexAttributeType::Float;
    else if constexpr (IsIntVertexAttribute<T>)
        return VertexAttributeType::Int;
    else if constexpr (IsUIntVertexAttribute<T>)
        return VertexAttributeType::UInt;
    else
        static_assert(false, "Unsupported vertex attribute type");
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
        static_assert(false, "Unsupported vertex attribute type");
}

template <typename T, typename U>
inline std::size_t memberOffset(U T::*ptr) noexcept
{
    constexpr T* nullObj = nullptr;
    return std::bit_cast<std::size_t>(&(nullObj->*ptr));
}

} // namespace vertex_traits

template <typename VertexType, typename MemberType>
inline constexpr VertexAttribute generateAttribute(int location,
                                                   const char* name,
                                                   MemberType(VertexType::*ptr),
                                                   bool normalized = false) noexcept
{
    static_assert(vertex_traits::SupportedVertexAttribute<MemberType>, "Unsupported vertex attribute type");

    const std::size_t offset = vertex_traits::memberOffset(ptr);

    return {location,
            vertex_traits::getComponentCount<MemberType>(),
            offset,
            vertex_traits::getAttributeType<MemberType>(),
            normalized,
            name};
}

template <typename AttributeType>
inline constexpr VertexAttribute generateAttribute(int location, const char* name, bool normalized = false) noexcept
{
    return {location,
            vertex_traits::getComponentCount<AttributeType>(),
            0,
            vertex_traits::getAttributeType<AttributeType>(),
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
