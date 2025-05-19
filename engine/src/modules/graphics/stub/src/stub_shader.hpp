#pragma once

#include <string>
#include <unordered_map>

#include <engine/common_types.hpp>
#include <engine/graphics/shader.hpp>

namespace game_engine::graphics
{

class StubShader final : public graphics::IShader
{
public:

    // graphics::IShader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void clear() noexcept override;
    bool isValid() const noexcept override;
};

} // namespace game_engine::graphics
