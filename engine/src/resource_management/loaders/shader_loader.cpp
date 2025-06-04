#include "shader_loader.hpp"

#include <fstream>

#include <resource_management/resources/shader_resource.hpp>

namespace fs = std::filesystem;

namespace game_engine
{

std::string LoadFile(const fs::path path)
{
    const auto abs_path = fs::absolute(path);

    if (!fs::exists(abs_path)) {
        throw std::runtime_error("File not exists: " + abs_path.string());
    }

    std::ifstream file(abs_path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + abs_path.string());
    }

    const auto file_size = file.tellg();
    if (file_size == -1) {
        throw std::runtime_error("Failed to read file size: " + abs_path.string());
    }

    std::string content;
    content.resize(file_size);

    if (file_size > 0) {
        file.seekg(0, std::ios::beg);
        file.read(content.data(), file_size);
        if (!file) {
            throw std::runtime_error("Failed to read file contents: " + abs_path.string());
        }
    }

    return content;
}

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
            shader->SetSource(t, LoadFile(it->second));
        }
    }

    return shader;
}

} // namespace game_engine
