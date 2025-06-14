#include "mesh_loader.hpp"

#include <filesystem>

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

    return mesh;
}

} // namespace game_engine
