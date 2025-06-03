#include "shader_impl.hpp"

namespace game_engine
{

ShaderImpl::ShaderImpl(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool ShaderImpl::Load()
{
    bool is_data_ready = true;

    is_data_ready &= m_sources.count(ShaderType::Vertex) > 0;
    is_data_ready &= m_sources.count(ShaderType::Fragment) > 0;
    is_data_ready &= m_state != ResourceState::Error;

    if (is_data_ready) {
        m_state = ResourceState::LoadedInRAM;
    }

    return is_data_ready;
}

bool ShaderImpl::Unload()
{
    m_state = ResourceState::NotLoaded;

    return true;
}

bool ShaderImpl::Reload()
{
    if (m_state == ResourceState::LoadedInGPU) {
        m_state = ResourceState::Loading;
    }

    return IsReady();
}

bool ShaderImpl::IsReady() const noexcept
{
    return m_state == ResourceState::LoadedInGPU;
}

ResourceId ShaderImpl::GetId() const noexcept
{
    return m_id;
}

std::string ShaderImpl::GetName() const noexcept
{
    return m_name;
}

ResourceType ShaderImpl::GetType() const noexcept
{
    return ResourceType::Shader;
}

ResourceState ShaderImpl::GetState() const noexcept
{
    return m_state;
}

void ShaderImpl::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> ShaderImpl::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region IShader implementation

void ShaderImpl::SetSource(ShaderType type, const std::string& source)
{
    m_sources[type] = source;
}

const std::string& ShaderImpl::GetSource(ShaderType type)
{
    return m_sources[type];
}

#pragma endregion

} // namespace game_engine
