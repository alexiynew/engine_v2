#include "stub_shader.hpp"

namespace game_engine::graphics
{

void StubShader::SetSource(const std::string& vertexSource, const std::string& fragmentSource)
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
