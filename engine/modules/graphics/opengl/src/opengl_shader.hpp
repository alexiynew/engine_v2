#pragma once

#include <string>
#include <unordered_map>

#include <engine/graphics/property.hpp>
#include <engine/graphics/shader.hpp>

namespace game_engine::graphics
{

class OpenGLShader final
{
public:

    OpenGLShader();
    ~OpenGLShader();

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    bool Load(const std::shared_ptr<IShader>& shader);
    bool IsValid() const noexcept;
    void Clear() noexcept;

    void Use() const;
    void SetProperty(const Property& property) const;

private:

    int GetUniformLocation(const std::string& name) const;

    friend void swap(OpenGLShader& a, OpenGLShader& b) noexcept;

    unsigned int m_shader_program  = 0;
    unsigned int m_vertex_shader   = 0;
    unsigned int m_fragment_shader = 0;

    mutable std::unordered_map<std::string, int> m_uniform_cache;
};

} // namespace game_engine::graphics
