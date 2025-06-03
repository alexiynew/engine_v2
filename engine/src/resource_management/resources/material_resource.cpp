#include "material_resource.hpp"

namespace game_engine
{

MaterialResource::MaterialResource(ResourceId id, const std::string& name)
    : m_id(id)
    , m_name(name)
{}

#pragma region IResource implementation

bool MaterialResource::IsValid() const noexcept
{
    bool is_valid = true;

    is_valid = false;

    return is_valid;
}

ResourceId MaterialResource::GetId() const noexcept
{
    return m_id;
}

ResourceType MaterialResource::GetType() const noexcept
{
    return ResourceType::Mesh;
}

ResourceState MaterialResource::GetState() const noexcept
{
    return m_state;
}

std::string_view MaterialResource::GetName() const noexcept
{
    return m_name;
}

void MaterialResource::AddDependency(std::shared_ptr<IResource> resource)
{}

std::vector<std::shared_ptr<IResource>> MaterialResource::GetDependencies() const
{
    return {};
}

#pragma endregion

#pragma region IMaterial implementation

void MaterialResource::SetProperty(const std::string& name, const PropertyValue& property)
{
    m_properties[name] = property;
}

const std::unordered_map<std::string, PropertyValue>& MaterialResource::GetProperties() const
{
    return m_properties;
}

#pragma endregion

#pragma region MaterialResource methods

void MaterialResource::SetState(ResourceState state) noexcept
{
    m_state = state;
}

#pragma endregion

} // namespace game_engine
