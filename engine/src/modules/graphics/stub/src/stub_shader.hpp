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
    void SetSource(const std::string& vertex_source, const std::string& fragment_source) override;
    bool Link() override;
    void Clear() noexcept override;
    bool IsValid() const noexcept override;
};

} // namespace game_engine::graphics
