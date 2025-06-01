#pragma once

#include <engine/graphics/mesh_data.hpp>
#include <engine/graphics/vertex_traits.hpp>
#include <engine/resource.hpp>

namespace game_engine
{

struct MeshLoadParams
{
    std::string source;
    std::vector<std::string> additional_files;
    PrimitiveType primitive_type = PrimitiveType::Triangles;
    VertexLayout layout;
};

class IMesh : public IResource
{
public:

    ~IMesh() override = default;

    virtual void SetMeshData(const MeshData& data)           = 0;
    virtual void SetPrimitiveType(PrimitiveType type)        = 0;
    virtual void SetVertexLayout(const VertexLayout& layout) = 0;
    virtual bool LoadToGpu()                                 = 0;
};

} // namespace game_engine
