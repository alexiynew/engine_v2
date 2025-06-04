#pragma once

#include <unordered_map>

#include <engine/graphics/shader.hpp>

namespace game_engine
{
class ShaderResource final : public IShader
{
public:

    ShaderResource(ResourceId id, const std::string& name);

    // IResource implementation
    bool IsValid() const noexcept override;
    ResourceId GetId() const noexcept override;
    ResourceType GetType() const noexcept override;
    ResourceState GetState() const noexcept override;
    std::string_view GetName() const noexcept override;

    void AddDependency(std::shared_ptr<IResource> resource) override;
    std::vector<std::shared_ptr<IResource>> GetDependencies() const override;

    // IShader implementation
    void SetSource(ShaderType type, const std::string& source) override;
    const std::string& GetSource(ShaderType type) override;

    // ShaderResource methods
    void SetState(ResourceState state) noexcept;

private:

    ResourceId m_id;
    std::string m_name;
    ResourceState m_state = ResourceState::NotLoaded;

    std::unordered_map<ShaderType, std::string> m_sources;
};

} // namespace game_engine
