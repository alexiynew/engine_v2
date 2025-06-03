#pragma once

#include <engine/graphics/shader.hpp>

namespace game_engine
{
class ShaderImpl final : public IShader
{
public:

    ShaderImpl(ResourceId id, const std::string& name);

    // IResource implementation
    bool Load() override;
    bool Unload() override;
    bool Reload() override;

    bool IsReady() const noexcept override;
    ResourceId GetId() const noexcept override;
    std::string GetName() const noexcept override;
    ResourceType GetType() const noexcept override;
    ResourceState GetState() const noexcept override;

    void AddDependency(std::shared_ptr<IResource> resource) override;
    std::vector<std::shared_ptr<IResource>> GetDependencies() const override;

    // IShader implementation
    void SetSource(ShaderType type, const std::string& source) override;
    const std::string& GetSource(ShaderType type) override;

private:

    ResourceId m_id;
    std::string m_name;
    ResourceState m_state = ResourceState::NotLoaded;

    std::unordered_map<ShaderType, std::string> m_sources;
};

} // namespace game_engine
