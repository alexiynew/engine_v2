#pragma once

#include <game_engine/core/mesh_data.hpp>

namespace game_engine::core
{

namespace vertex_traits
{
template <typename T>
constexpr VertexAttributeType getAttributeType()
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
constexpr int getComponentCount()
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

} // namespace vertex_traits

template <typename VertexType, auto MemberPtr, std::size_t Offset>
constexpr VertexAttribute generateAttribute(int location, const char* name, bool normalized = false)
{
    using MemberType = std::decay_t<decltype(VertexType{}.*MemberPtr)>;

    static constexpr auto isFloat = std::is_same_v<MemberType, Vector2> || std::is_same_v<MemberType, Vector3> ||
                                    std::is_same_v<MemberType, Vector4>;

    static constexpr auto isInt = std::is_same_v<MemberType, Vector2i> || std::is_same_v<MemberType, Vector3i> ||
                                  std::is_same_v<MemberType, Vector4i>;

    static constexpr auto isUInt = std::is_same_v<MemberType, Vector2u> || std::is_same_v<MemberType, Vector3u> ||
                                   std::is_same_v<MemberType, Vector4u>;

    using ScalarType = std::
    conditional_t<isFloat, float, std::conditional_t<isInt, int, std::conditional_t<isUInt, unsigned int, void>>>;

    static_assert(!std::is_same_v<ScalarType, void>, "Unsupported vertex scalar type");

    return {location,
            vertex_traits::getComponentCount<MemberType>(),
            Offset,
            vertex_traits::getAttributeType<ScalarType>(),
            normalized,
            name};
}

/// @brief Create VertexLayout for VertexType
/// User must provide specialization of this function for their VertexType
/// @return VertexLayout for VertexType
template <typename VertexType>
inline VertexLayout getVertexLayout()
{
    static_assert(sizeof(VertexType) == 0, "You must provide getVertexLayout specialization for your vertex type");
    return {};
}

template <>
inline VertexLayout getVertexLayout<Vector3>()
{
    return {.vertexSize = sizeof(Vector3),
            .attributes{{.location   = 0,
                         .components = 3,
                         .offset     = 0,
                         .type       = VertexAttributeType::Float,
                         .normalized = false,
                         .name       = "position"}}};
}

template <typename VertexType>
MeshData createMeshData(const std::vector<VertexType>& vertices,
                        const std::vector<SubMesh>& submeshes,
                        PrimitiveType primitiveType)
{
    static_assert(std::is_standard_layout_v<VertexType>, "Vertex type must be standard layout");

    MeshData data = {.vertexData    = {},
                     .vertexCount   = vertices.size(),
                     .submeshes     = submeshes,
                     .instances     = {},
                     .primitiveType = primitiveType,
                     .layout        = getVertexLayout<VertexType>()};

    data.vertexData.resize(vertices.size() * sizeof(VertexType));
    std::memcpy(data.vertexData.data(), vertices.data(), data.vertexData.size());

    return data;
}

} // namespace game_engine::core
