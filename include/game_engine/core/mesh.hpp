#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <game_engine/common_types.hpp>

namespace game_engine::core
{

/// @brief Represents a vertex with position, normal, UV coordinates, and color.
struct Vertex
{
    Vector3 position; ///< Vertex position in 3D space.
    Vector3 normal;   ///< Normal vector for lighting calculations.
    Vector2 uv;       ///< Texture coordinates (UV mapping).
    Vector3 color;    ///< Vertex color (RGB).
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

/// @brief Represents a bounding box for a mesh.
struct BoundingBox
{
    Vector3 min; ///< Minimum coordinates of the bounding box.
    Vector3 max; ///< Maximum coordinates of the bounding box.

    /// @brief Returns the center of the bounding box.
    /// @return The center of the bounding box as a Vector3.
    Vector3 getCenter() const;

    /// @brief Returns the size of the bounding box.
    /// @return The size of the bounding box as a Vector3.
    Vector3 getSize() const;
};

/// @brief Type alias for mesh identifier.
using MeshId = std::size_t;

/// @brief Represents a mesh with vertices, submeshes, instances, and other properties.
struct Mesh
{
    std::vector<Vertex> vertices;        ///< List of vertices in the mesh.
    std::vector<SubMesh> subMeshes;      ///< List of submeshes (each with its own material).
    std::vector<InstanceData> instances; ///< List of instances for instanced rendering.
    PrimitiveType primitiveType;         ///< Type of primitives used for rendering.
    BoundingBox boundingBox;             ///< Bounding box for the mesh.
    bool isTransparent = false;          ///< Flag indicating if the mesh is transparent.
    MeshId id;                           ///< Unique identifier for the mesh.

    /// @brief Calculates the bounding box based on the vertices.
    void calculateBoundingBox();
};

} // namespace game_engine::core
