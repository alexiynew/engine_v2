#pragma once

#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/shader.hpp>

namespace game_engine::backend
{

class OpenGLShader final : public core::Shader
{
public:
    OpenGLShader() noexcept;
    ~OpenGLShader() noexcept override;

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    // core::Shader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void setUniform(const std::string& name, const core::Uniform& uniform) override;
    void clear() noexcept override;
    bool isValid() const noexcept override;

    void use() const;

    void bindAttributeLocation(std::uint32_t location, const std::string& name) const;
    int getAttributeLocation(const std::string& name) const;

private:
    std::string m_vertexSource;
    std::string m_fragmentSource;

    std::uint32_t m_shaderProgram  = 0;
    std::uint32_t m_vertexShader   = 0;
    std::uint32_t m_fragmentShader = 0;

    mutable std::unordered_map<std::string, int> m_uniformCache;

    friend void swap(OpenGLShader& a, OpenGLShader& b);

    int getUniformLocation(const std::string& name) const;
};

} // namespace game_engine::backend
