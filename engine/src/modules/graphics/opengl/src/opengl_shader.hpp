#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <engine/common_types.hpp>
#include <engine/graphics/shader.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

class OpenGLShader final : public graphics::IShader
{
public:
    explicit OpenGLShader(std::shared_ptr<OpenGLRenderer> renderThread) noexcept;
    ~OpenGLShader() override;

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    // graphics::Shader
    void setSource(const std::string& vertexSource, const std::string& fragmentSource) override;
    bool link() override;
    void clear() noexcept override;
    bool isValid() const noexcept override;

    void setUniform(const graphics::Uniform& uniform) const;
    void use() const;

    void bindAttributeLocation(std::uint32_t location, const std::string& name) const;
    int getAttributeLocation(const std::string& name) const;

private:
    friend void swap(OpenGLShader& a, OpenGLShader& b) noexcept;

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

} // namespace game_engine::graphics
