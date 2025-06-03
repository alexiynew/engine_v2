#include "mesh_loader.hpp"

#include <vector>

#include <engine/common_types.hpp>
#include <engine/graphics/vertex_traits.hpp>

#include <graphics/mesh_impl.hpp>

namespace
{

struct Vertex
{
    game_engine::Vector3 position;
    game_engine::Vector3 normal;
    game_engine::Vector2 uv;
    game_engine::Vector4 color;
};

std::vector<Vertex> vertices = {
    // Front edge (red)
    Vertex{ {-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 0
    Vertex{  {0.5f, -0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 1
    Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 2
    Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Vertex 3

    //Back edge (green)
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 4
    Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 5
    Vertex{  {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 6
    Vertex{ {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 7

    // Upper edge (blue)
    Vertex{  {-0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 8
    Vertex{   {0.5f, 0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 9
    Vertex{  {0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 10
    Vertex{ {-0.5f, 0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 11

    // Bottom edge (yellow)
    Vertex{ {-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 12
    Vertex{  {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 13
    Vertex{ {0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 14
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 15

    // Left edge (blue)
    Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 16
    Vertex{ {-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 17
    Vertex{  {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 18
    Vertex{ {-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}}, // Vertex 19

    // Right edge (purple)
    Vertex{ {0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 20
    Vertex{  {0.5f, -0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 21
    Vertex{   {0.5f, 0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 22
    Vertex{  {0.5f, 0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}}, // Vertex 23
};

std::vector<unsigned int> submesh_indices =
{0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

} // namespace

namespace game_engine
{

// https://www.martinreddy.net/gfx/3d/OBJ.spec
// https://paulbourke.net/dataformats/obj/
std::shared_ptr<IMesh> MeshLoader::Load(ResourceId id, const std::string_view name, const MeshLoadParams& params) const
{
    auto mesh = std::make_shared<MeshImpl>(id, std::string(name));

    mesh->SetVertexData(vertex_traits::ConvertToVertexData(vertices,
    {
        vertex_traits::GenerateAttribute(0, "position", &Vertex::position),
        vertex_traits::GenerateAttribute(1, "normal", &Vertex::normal),
        vertex_traits::GenerateAttribute(2, "uv", &Vertex::uv),
        vertex_traits::GenerateAttribute(3, "color", &Vertex::color),
    }));

    mesh->AddSubMesh({submesh_indices, nullptr});

    mesh->Load();

    return mesh;
}

} // namespace game_engine
