#pragma once

#include <engine/graphics/material.hpp>

namespace game_engine
{

class MaterialResource final : public IMaterial
{
public:

    MaterialResource(ResourceId id, const std::string& name);

    // IResource implementation
    bool IsValid() const noexcept override;
    ResourceId GetId() const noexcept override;
    ResourceType GetType() const noexcept override;
    ResourceState GetState() const noexcept override;
    std::string_view GetName() const noexcept override;

    void AddDependency(std::shared_ptr<IResource> resource) override;
    std::vector<std::shared_ptr<IResource>> GetDependencies() const override;

    // IMaterial implementation
    void SetProperty(const std::string& name, const PropertyValue& property) override;
    const std::unordered_map<std::string, PropertyValue>& GetProperties() const override;

    // MaterialResource methods
    void SetState(ResourceState state) noexcept;

private:

    const ResourceId m_id = 0;
    std::string m_name;
    ResourceState m_state = ResourceState::NotLoaded;

    std::unordered_map<std::string, PropertyValue> m_properties;
};

} // namespace game_engine
