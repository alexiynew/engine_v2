#include "shader_resource.hpp"

namespace game_engine
{

ShaderResource::ShaderResource(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool ShaderResource::IsValid() const noexcept
{
    bool is_valid = true;

    is_valid &= m_sources.count(ShaderType::Vertex) > 0;
    is_valid &= m_sources.count(ShaderType::Fragment) > 0;
    is_valid &= m_state != ResourceState::Error;

    return is_valid;
}

ResourceId ShaderResource::GetId() const noexcept
{
    return m_id;
}

ResourceType ShaderResource::GetType() const noexcept
{
    return ResourceType::Shader;
}

ResourceState ShaderResource::GetState() const noexcept
{
    return m_state;
}

std::string_view ShaderResource::GetName() const noexcept
{
    return m_name;
}

void ShaderResource::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> ShaderResource::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region IShader implementation

void ShaderResource::SetSource(ShaderType type, const std::string& source)
{
    m_sources[type] = source;
}

const std::string& ShaderResource::GetSource(ShaderType type)
{
    return m_sources[type];
}

#pragma endregion

#pragma region ShaderResource methods

void ShaderResource::SetState(ResourceState state) noexcept
{
    m_state = state;
}

#pragma endregion

} // namespace game_engine
