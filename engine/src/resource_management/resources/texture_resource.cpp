#include "texture_resource.hpp"

namespace game_engine
{

TextureResource::TextureResource(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool TextureResource::IsValid() const noexcept
{
    bool is_valid = true;

    is_valid = false;

    return is_valid;
}

ResourceId TextureResource::GetId() const noexcept
{
    return m_id;
}

ResourceType TextureResource::GetType() const noexcept
{
    return ResourceType::Mesh;
}

ResourceState TextureResource::GetState() const noexcept
{
    return m_state;
}

std::string_view TextureResource::GetName() const noexcept
{
    return m_name;
}

void TextureResource::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> TextureResource::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region ITexture implementation

#pragma endregion

#pragma region TextureResource methods

void TextureResource::SetState(ResourceState state) noexcept
{
    m_state = state;
}

#pragma endregion

} // namespace game_engine
