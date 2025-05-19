#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <engine/common_types.hpp>

namespace game_engine::graphics
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
    int location             = 0;                          ///< Location (index) of the attribute in the shader.
    int components           = 0;                          ///< Number of components (e.g., 3 for Vector3).
    std::size_t offset       = 0;                          ///< Offset in bytes from the start of the vertex.
    VertexAttributeType type = VertexAttributeType::Float; ///< Data type of attribute.
    bool normalized          = false;                      ///< Whether the attribute should be normalized.
    const char* name         = nullptr;                    ///< Name of the attribute (e.g., "position", "normal").
};

/// @brief Vertex format description
struct VertexLayout
{
    std::size_t vertex_size = 0;             ///< Vertex size in bytes.
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
    Matrix4 model_matrix; ///< Transformation matrix for the instance.
    Vector4 color;        ///< Additional data (e.g., color) for the instance.
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
    std::vector<std::uint8_t> vertex_data; ///< List of vertices in the mesh.
    size_t vertex_count = 0;

    std::vector<SubMesh> submeshes;                          ///< List of submeshes (each with its own material).
    std::vector<InstanceData> instances;                     ///< List of instances for instanced rendering.
    PrimitiveType primitive_type = PrimitiveType::Triangles; ///< Type of primitives used for rendering.
    VertexLayout layout;                                     ///< vertex layout,
};

} // namespace game_engine::graphics
