#include "stub_shader.hpp"

namespace game_engine::graphics
{

void StubShader::SetSource(const std::string& vertex_source, const std::string& fragment_source)
{}

bool StubShader::Link()
{
    return true;
}

void StubShader::Clear() noexcept
{}

bool StubShader::IsValid() const noexcept
{
    return true;
}

} // namespace game_engine::graphics
