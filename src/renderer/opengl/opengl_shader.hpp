#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>
#include <game_engine/core/shader.hpp>

#include <renderer/opengl/opengl_renderer.hpp>

namespace game_engine::renderer
{

class OpenGLShader final : public core::Shader
{
public:
    explicit OpenGLShader(std::shared_ptr<OpenGLRenderer> renderThread) noexcept;
    ~OpenGLShader() override;

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    // core::Shader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void clear() noexcept override;
    bool isValid() const noexcept override;

    void setUniform(const core::Uniform& uniform) const;
    void use() const;

    void bindAttributeLocation(std::uint32_t location, const std::string& name) const;
    int getAttributeLocation(const std::string& name) const;

private:
    friend void swap(OpenGLShader& a, OpenGLShader& b);

    int getUniformLocation(const std::string& name) const;

    bool linkImpl();

    std::shared_ptr<OpenGLRenderer> m_renderer;

    std::string m_vertexSource;
    std::string m_fragmentSource;

    std::uint32_t m_shaderProgram  = 0;
    std::uint32_t m_vertexShader   = 0;
    std::uint32_t m_fragmentShader = 0;

    mutable std::unordered_map<std::string, int> m_uniformCache;
};

} // namespace game_engine::renderer
