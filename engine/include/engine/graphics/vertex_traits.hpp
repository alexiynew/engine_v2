#pragma once

#include <bit>
#include <cstring>
#include <type_traits>

#include <engine/graphics/mesh_data.hpp>

namespace game_engine
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
constexpr VertexAttributeType GetAttributeType() noexcept
{
    if constexpr (IsFloatVertexAttribute<T>) {
        return VertexAttributeType::Float;
    } else if constexpr (IsIntVertexAttribute<T>) {
        return VertexAttributeType::Int;
    } else if constexpr (IsUIntVertexAttribute<T>) {
        return VertexAttributeType::UInt;
    } else {
        static_assert(!std::is_same_v<T, T>, "Unsupported vertex attribute type");
    }
}

template <typename T>
constexpr int GetComponentCount() noexcept
{
    static_assert(SupportedVertexAttribute<T>, "Unsupported vertex attribute type");

    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, unsigned int>) {
        return 1; // for scalar types
    } else if constexpr (std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector2u>) {
        return 2;
    } else if constexpr (std::is_same_v<T, Vector3> || std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector3u>) {
        return 3;
    } else if constexpr (std::is_same_v<T, Vector4> || std::is_same_v<T, Vector4i> || std::is_same_v<T, Vector4u>) {
        return 4;
    } else {
        static_assert(!std::is_same_v<T, T>, "Unsupported vertex attribute type");
    }
}

template <typename T, typename U>
std::size_t MemberOffset(U T::* ptr) noexcept
{
    constexpr T* NullObj = nullptr;
    return std::bit_cast<std::size_t>(&(NullObj->*ptr));
}

} // namespace vertex_traits

template <typename TVertexType, typename TMemberType>
constexpr VertexAttribute GenerateAttribute(int location,
                                            const char* name,
                                            TMemberType(TVertexType::* ptr),
                                            bool normalized = false) noexcept
{
    static_assert(vertex_traits::SupportedVertexAttribute<TMemberType>, "Unsupported vertex attribute type");

    const std::size_t offset = vertex_traits::MemberOffset(ptr);

    return {location,
            vertex_traits::GetComponentCount<TMemberType>(),
            offset,
            vertex_traits::GetAttributeType<TMemberType>(),
            normalized,
            name};
}

template <typename TAttributeType>
constexpr VertexAttribute GenerateAttribute(int location, const char* name, bool normalized = false) noexcept
{
    return {location,
            vertex_traits::GetComponentCount<TAttributeType>(),
            0,
            vertex_traits::GetAttributeType<TAttributeType>(),
            normalized,
            name};
}

template <typename TVertexType>
MeshData CreateMeshData(const std::vector<TVertexType>& vertices,
                        const std::vector<SubMesh>& submeshes,
                        PrimitiveType primitive_type,
                        VertexLayout&& layout)
{
    static_assert(std::is_standard_layout_v<TVertexType>, "Vertex type must be standard layout");

    MeshData data = {.vertex_data    = {},
                     .vertex_count   = vertices.size(),
                     .submeshes      = submeshes,
                     .instances      = {},
                     .primitive_type = primitive_type,
                     .layout         = std::move(layout)};

    data.vertex_data.resize(vertices.size() * sizeof(TVertexType));
    std::memcpy(data.vertex_data.data(), vertices.data(), data.vertex_data.size());

    return data;
}

} // namespace game_engine
