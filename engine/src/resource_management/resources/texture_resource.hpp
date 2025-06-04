#pragma once

#include <engine/graphics/texture.hpp>

namespace game_engine
{

class TextureResource final : public ITexture
{
public:

    TextureResource(ResourceId id, const std::string& name);

    // IResource implementation
    bool IsValid() const noexcept override;
    ResourceId GetId() const noexcept override;
    ResourceType GetType() const noexcept override;
    ResourceState GetState() const noexcept override;
    std::string_view GetName() const noexcept override;

    void AddDependency(std::shared_ptr<IResource> resource) override;
    std::vector<std::shared_ptr<IResource>> GetDependencies() const override;

    // ITexture implementation

    // TextureResource methods
    void SetState(ResourceState state) noexcept;

private:

    const ResourceId m_id = 0;
    std::string m_name;
    ResourceState m_state = ResourceState::NotLoaded;
};

} // namespace game_engine
