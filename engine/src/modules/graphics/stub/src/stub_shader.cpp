#include "stub_shader.hpp"

namespace game_engine::graphics
{

void StubShader::setSource(const std::string& vertexSource, const std::string& fragmentSource)
{}

bool StubShader::link()
{
    return true;
}

void StubShader::clear() noexcept
{}

bool StubShader::isValid() const noexcept
{
    return true;
}

} // namespace game_engine::graphics
