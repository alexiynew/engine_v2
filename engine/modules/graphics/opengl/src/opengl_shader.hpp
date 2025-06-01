#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <engine/common_types.hpp>

#include <opengl_renderer.hpp>

namespace game_engine::graphics
{

class OpenGLShader final
{
public:

    explicit OpenGLShader(std::shared_ptr<OpenGLRenderer> render_thread) noexcept;
    ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    void SetSource(const std::string& vertex_source, const std::string& fragment_source);
    bool LoadToGpu();
    void Clear() noexcept;
    bool IsValid() const noexcept;

    //void SetProperty(const Property& property) const;
    void Use() const;

    void BindAttributeLocation(std::uint32_t location, const std::string& name) const;
    int GetAttributeLocation(const std::string& name) const;

private:

    friend void swap(OpenGLShader& a, OpenGLShader& b) noexcept;

    int GetUniformLocation(const std::string& name) const;

    bool LinkImpl();

    std::shared_ptr<OpenGLRenderer> m_renderer;

    std::string m_vertex_source;
    std::string m_fragment_source;

    std::uint32_t m_shader_program  = 0;
    std::uint32_t m_vertex_shader   = 0;
    std::uint32_t m_fragment_shader = 0;

    mutable std::unordered_map<std::string, int> m_uniform_cache;
};

} // namespace game_engine::graphics
