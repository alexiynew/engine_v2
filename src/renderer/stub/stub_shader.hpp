#pragma once

#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/shader.hpp>

namespace game_engine::renderer
{

class StubShader final : public core::Shader
{
public:

    // core::Shader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void clear() noexcept override;
    bool isValid() const noexcept override;
};

} // namespace game_engine::renderer
