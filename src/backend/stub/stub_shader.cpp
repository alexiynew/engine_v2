#include "stub_shader.hpp"

namespace game_engine::backend
{

void StubShader::setSource(const std::string& vertexSource, const std::string& fragmentSource)
{}

bool StubShader::link()
{
    return true;
}

void StubShader::use() const
{}

void StubShader::setUniform(const std::string& name, const core::Uniform& uniform)
{}

void StubShader::clear()
{}

bool StubShader::isValid() const
{
    return true;
}

} // namespace game_engine::backend
