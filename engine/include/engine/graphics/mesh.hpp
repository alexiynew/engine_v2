#pragma once

#include <engine/graphics/material.hpp>
#include <engine/resource.hpp>

namespace game_engine
{

/// @brief Enumeration for primitive types used in rendering.
enum class PrimitiveType
{
    Triangles,     ///< Triangles (3 vertices per primitive).
    TriangleStrip, ///< Triangle strip (sequence of triangles).
    TriangleFan,   ///< Triangle fan (all triangles share one vertex).
    Lines,         ///< Lines (2 vertices per primitive).
    LineStrip,     ///< Line strip (sequence of lines).
    LineLoop,      ///< Line loop (closed line strip).
    Points,        ///< Points (1 vertex per primitive).
};

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

/// @brief Represents a submesh with indices and material.
struct SubMesh
{
    std::vector<unsigned int> indices;   ///< Indices for this submesh.
    std::shared_ptr<IMaterial> material; ///< Material applied to this submesh.
};

struct MeshLoadParams
{
    std::string source;
    std::vector<std::string> additional_files;
    PrimitiveType primitive_type = PrimitiveType::Triangles;
};

struct VertexData
{
    std::size_t vertex_count = 0;
    std::size_t vertex_size  = 0;            ///< Vertex size in bytes.
    std::vector<VertexAttribute> attributes; ///< Vertex attributes list.
    std::vector<std::byte> data;
};

class IMesh : public IResource
{
public:

    ~IMesh() override = default;

    virtual void SetVertexData(VertexData data)       = 0;
    virtual void AddSubMesh(SubMesh submesh)          = 0;
    virtual void SetPrimitiveType(PrimitiveType type) = 0;

    virtual const VertexData& GetVertexData() const          = 0;
    virtual const std::vector<SubMesh>& GetSubMeshes() const = 0;
    virtual PrimitiveType GetPrimitiveType() const           = 0;
};

} // namespace game_engine
