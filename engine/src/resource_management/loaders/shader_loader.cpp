#include "shader_loader.hpp"

#include <resource_management/loaders/file_funcitons.hpp>
#include <resource_management/resources/shader_resource.hpp>

namespace game_engine
{

std::shared_ptr<ShaderResource> ShaderLoader::Load(ResourceId id, std::string_view name, const ShaderLoadParams& params) const
{
    auto shader = std::make_shared<ShaderResource>(id, std::string(name));

    for (auto t : {ShaderType::Vertex,
             ShaderType::Fragment,
             ShaderType::Geometry,
             ShaderType::TessControl,
             ShaderType::TessEvaluation,
             ShaderType::Compute})
    {
        if (auto it = params.source_files.find(t); it != params.source_files.end()) {
            shader->SetSource(t, fs::LoadFile(it->second));
        }
    }

    return shader;
}

} // namespace game_engine
