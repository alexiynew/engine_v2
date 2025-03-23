#pragma once

#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/shader.hpp>

namespace game_engine::backend
{

class StubShader final : public core::Shader
{
public:

    // core::Shader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void use() const override;
    void setUniform(const std::string& name, const core::Uniform& uniform) override;
    void clear() override;
    bool isValid() const override;
};

} // namespace game_engine::backend
