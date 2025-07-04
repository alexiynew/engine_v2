#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <engine/resource_management/resource.hpp>

namespace game_engine
{

enum class ShaderType
{
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEvaluation,
    Compute,
};

struct ShaderLoadParams
{
    std::unordered_map<ShaderType, std::filesystem::path> source_files;
};

class IShader : public IResource
{
public:

    ~IShader() override = default;

    virtual void SetSource(ShaderType type, const std::string& source) = 0;
    virtual const std::string& GetSource(ShaderType type)              = 0;
};

} // namespace game_engine
