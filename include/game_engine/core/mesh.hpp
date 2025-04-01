#pragma once

#include <array>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include <game_engine/common_types.hpp>

namespace game_engine::core
{

/// @brief Vertex attribute data type
enum class VertexAttributeType
{
    Float,
    Int,
    UInt,
};

/// @brief Represents a vertex attribute with its location and properties.
struct VertexAttribute
{
    int location;             ///< Location (index) of the attribute in the shader.
    int components;           ///< Number of components (e.g., 3 for Vector3).
    std::size_t offset;       ///< Offset in bytes from the start of the vertex.
    VertexAttributeType type; ///< Data type of attribute.
    bool normalized;          ///< Whether the attribute should be normalized.
    const char* name;         ///< Name of the attribute (e.g., "position", "normal").
};

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
constexpr VertexAttribute generateAttribute(int Location, const char* name)
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

    return {Location,
            vertex_traits::getComponentCount<MemberType>(),
            Offset,
            vertex_traits::getAttributeType<ScalarType>(),
            std::is_floating_point_v<ScalarType>, // normalized only for float
            name};
}

/// @brief Vertex format description
struct VertexLayout
{
    std::size_t vertexSize;                  ///< Vertex size in bytes.
    std::vector<VertexAttribute> attributes; ///< Vertex attributes list.
};

/// @brief Represents a material with various parameters (e.g., textures, colors, etc.).
struct Material
{
    std::unordered_map<std::string, std::variant<float, std::string, Vector3>> params; ///< Material parameters.
};

/// @brief Represents a submesh with indices and material.
struct SubMesh
{
    std::vector<unsigned int> indices; ///< Indices for this submesh.
    Material material;                 ///< Material applied to this submesh.
};

/// @brief Represents instance data for instanced rendering.
struct InstanceData
{
    glm::mat4 modelMatrix; ///< Transformation matrix for the instance.
    glm::vec4 color;       ///< Additional data (e.g., color) for the instance.
};

/// @brief Enumeration for primitive types used in rendering.
enum class PrimitiveType
{
    Triangles,     ///< Triangles (3 vertices per primitive).
    TriangleStrip, ///< Triangle strip (sequence of triangles).
    TriangleFan,   ///< Triangle fan (all triangles share one vertex).
    Lines,         ///< Lines (2 vertices per primitive).
    LineStrip,     ///< Line strip (sequence of lines).
    LineLoop,      ///< Line loop (closed line strip).
    Points         ///< Points (1 vertex per primitive).
};

/// @brief Mesh Data
struct MeshData
{
    std::vector<std::uint8_t> vertexData; ///< List of vertices in the mesh.
    size_t vertexCount;

    std::vector<SubMesh> submeshes;      ///< List of submeshes (each with its own material).
    std::vector<InstanceData> instances; ///< List of instances for instanced rendering.
    PrimitiveType primitiveType;         ///< Type of primitives used for rendering.
    VertexLayout layout;                 ///< vertex layout,
};

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual void setMeshData(const MeshData& data) = 0;
    virtual void flush()                           = 0;

    /// @brief Clears the mesh by removing it from the context.
    virtual void clear() = 0;

    /// @brief Checks if the mesh is valid and ready to be used.
    /// @return True if the mesh is valid, false otherwise.
    virtual bool isValid() const = 0;

    // TODO: Move render method in engine implementation.
    virtual void render() const = 0;
};

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
