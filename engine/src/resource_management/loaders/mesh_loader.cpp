#include "mesh_loader.hpp"

#include <filesystem>
#include <numeric>

#include <engine/common_types.hpp>
#include <engine/graphics/vertex_traits.hpp>

#include <resource_management/loaders/file_funcitons.hpp>
#include <resource_management/loaders/obj_parser.hpp>
#include <resource_management/resources/mesh_resource.hpp>

namespace game_engine
{

std::shared_ptr<MeshResource> MeshLoader::Load(ResourceId id, std::string_view name, const MeshLoadParams& params) const
{
    if (params.source.extension() != ".obj") {
        throw std::runtime_error("Unsupported file format");
    }

    ObjParser parser;
    if (!parser.Parse(fs::LoadFile(params.source))) {
        throw std::runtime_error("Failed to parse the " + std::string(name) + " data");
    }

    auto mesh = std::make_shared<MeshResource>(id, std::string(name));

    if (const auto& faces = parser.GetFaces(); !faces.empty()) {
        const bool has_normals   = !parser.GetNormals().empty();
        const bool has_texcoords = !parser.GetTextureVertices().empty();

        const std::size_t vertex_float_count = 3 + (has_normals ? 3 : 0) + (has_texcoords ? 2 : 0);
        const std::size_t total_indices = std::accumulate(faces.begin(), faces.end(), 0, [](std::size_t sum, const ObjParser::Face& face) {
            return face.size() < 3 ? sum : sum + (face.size() - 2) * 3;
        });

        std::unordered_map<std::size_t, std::uint32_t> vertex_map;
        vertex_map.reserve(total_indices);

        std::vector<float> vertex_data;
        vertex_data.reserve(total_indices * vertex_float_count);

        std::vector<std::uint32_t> indices;
        indices.reserve(total_indices);

        for (const auto& face : faces) {
            if (face.size() < 3) {
                continue;
            }

            // Polygon triangulation as triangle fan
            for (size_t i = 1; i < face.size() - 1; ++i) {
                const ObjParser::Triplet* triangles[] = {&face[0], &face[i], &face[i + 1]};

                for (const auto* triplet_ptr : triangles) {
                    const auto& [v, tv, n] = *triplet_ptr;

                    // TODO: Calc hash
                    const std::size_t key = 0;

                    if (auto it = vertex_map.find(key); it != vertex_map.end()) {
                        indices.push_back(it->second);
                    } else {
                        const std::uint32_t new_index = static_cast<std::uint32_t>(vertex_map.size());
                        vertex_map[key]               = new_index;
                        indices.push_back(new_index);

                        // Add position to vertex data
                        const auto& vertex = parser.GetVertices()[v];
                        vertex_data.insert(vertex_data.end(), {vertex.x, vertex.y, vertex.z});

                        // Add normal to vertex data
                        if (has_normals) {
                            if (n != ObjParser::InvalidIndex) {
                                const auto& normal = parser.GetNormals()[n];
                                vertex_data.insert(vertex_data.end(), {normal.i, normal.j, normal.k});
                            } else {
                                // Fill with zeros if no normal
                                vertex_data.insert(vertex_data.end(), {0.0f, 0.0f, 0.0f});
                            }
                        }

                        // Add texture coordinates to vertex data
                        if (has_texcoords) {
                            if (tv != ObjParser::InvalidIndex) {
                                const auto& tex = parser.GetTextureVertices()[tv];
                                vertex_data.insert(vertex_data.end(), {tex.u, tex.v});
                            } else {
                                // Fill with zeros if no texture coordinates
                                vertex_data.insert(vertex_data.end(), {0.0f, 0.0f});
                            }
                        }
                    }
                }
            }
        }

        // Create VertexData
        // TODO: Somehow simplify this
        VertexData vdata   = vertex_traits::ConvertToVertexData(vertex_data, {});
        vdata.vertex_count = vertex_map.size();
        vdata.vertex_size  = vertex_float_count * sizeof(float);
        vdata.data.resize(vertex_data.size() * sizeof(float));
        std::memcpy(vdata.data.data(), vertex_data.data(), vdata.data.size());

        // Attributes setup
        // TODO: Move attributes naming to clinet code
        // TODO: Move attributes location setup to clinet code
        size_t offset = 0;
        vdata.attributes.push_back(VertexAttribute{.location = 0,
            .components                                      = 3,
            .offset                                          = offset,
            .type                                            = VertexAttributeType::Float,
            .normalized                                      = false,
            .name                                            = "position"});
        offset += 3 * sizeof(float);

        if (has_normals) {
            vdata.attributes.push_back(VertexAttribute{.location = 1,
                .components                                      = 3,
                .offset                                          = offset,
                .type                                            = VertexAttributeType::Float,
                .normalized                                      = false,
                .name                                            = "normal"});
            offset += 3 * sizeof(float);
        }

        if (has_texcoords) {
            vdata.attributes.push_back(VertexAttribute{.location = 2,
                .components                                      = 2,
                .offset                                          = offset,
                .type                                            = VertexAttributeType::Float,
                .normalized                                      = false,
                .name                                            = "texCoord"});
        }

        // Создание подмеша
        SubMesh submesh;
        submesh.indices        = std::move(indices);
        submesh.primitive_type = PrimitiveType::Triangles;

        mesh->SetVertexData(std::move(vdata));
        mesh->AddSubMesh(std::move(submesh));
    }

    return mesh;
}

} // namespace game_engine
