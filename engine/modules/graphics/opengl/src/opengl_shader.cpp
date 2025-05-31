#include "opengl_shader.hpp"

#include <memory>
#include <string_view>

#include <glad/glad.h>
#include <opengl_utils.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

// TODO: move all opengl stuff in render thread
namespace
{
std::string ShaderTypeString(int shader_type)
{
    switch (shader_type) {
        case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
    }

    return "UNKNOWN_SHADER";
}

std::string ShaderInfoLog(GLuint shader_id)
{
    int length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return {};
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetShaderInfoLog(shader_id, length, nullptr, buffer.get());

    return {buffer.get()};
}

std::string ShaderProgramInfoLog(GLuint program_id)
{
    int length = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return {};
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetProgramInfoLog(program_id, length, nullptr, buffer.get());

    return {buffer.get()};
}

bool LoadShader(GLuint shader_id, int shader_type, const std::string& source)
{
    if (shader_id == 0) {
        return false;
    }

    const char* source_ptr = source.c_str();

    glShaderSource(shader_id, 1, &source_ptr, nullptr);
    glCompileShader(shader_id);

    int compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        LOG_ERROR << ShaderTypeString(shader_type) << " compilation error:\n" << ShaderInfoLog(shader_id) << std::endl;
        return false;
    }

    return true;
}

bool CompileShaderProgram(GLuint program_id, GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    if (program_id == 0 || vertex_shader_id == 0 || fragment_shader_id == 0) {
        return false;
    }

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    // mark shader for deletion
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    int linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
    if (linked == 0) {
        LOG_ERROR << "Shader program link error:\n" << ShaderProgramInfoLog(program_id) << std::endl;
        return false;
    }

    return true;
}

} // namespace

namespace game_engine::graphics
{

OpenGLShader::OpenGLShader(std::shared_ptr<OpenGLRenderer> render_thread) noexcept
    : m_renderer(std::move(render_thread))
{}

OpenGLShader::~OpenGLShader()
{
    Clear();
}

OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
{
    swap(*this, other);
}

OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept
{
    auto tmp = std::move(other);
    swap(*this, tmp);

    return *this;
}

void OpenGLShader::SetSource(const std::string& vertex_source, const std::string& fragment_source)
{
    m_vertex_source   = vertex_source;
    m_fragment_source = fragment_source;
}

bool OpenGLShader::LoadToGpu()
{
    auto result = m_renderer->SubmitSync([this]() {
        if (!LinkImpl()) {
            // TODO: report error
        }
    });

    try {
        result.get();
    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
    }
    return true;
}

void OpenGLShader::Use() const
{
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glUseProgram(m_shader_program);
}

void OpenGLShader::SetUniform(const Uniform& uniform) const
{
    const GLint location = GetUniformLocation(uniform.name);

    if (location < 0 || m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }

    std::visit([location](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, int>) {
            glUniform1i(location, arg);
        } else if constexpr (std::is_same_v<T, float>) {
            glUniform1f(location, arg);
        } else if constexpr (std::is_same_v<T, Vector2>) {
            glUniform2f(location, arg.x, arg.y);
        } else if constexpr (std::is_same_v<T, Vector3>) {
            glUniform3f(location, arg.x, arg.y, arg.z);
        } else if constexpr (std::is_same_v<T, Vector4>) {
            glUniform4f(location, arg.x, arg.y, arg.z, arg.w);
        } else if constexpr (std::is_same_v<T, Matrix3>) {
            glUniformMatrix3fv(location, 1, GL_FALSE, &arg[0][0]);
        } else if constexpr (std::is_same_v<T, Matrix4>) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &arg[0][0]);
        } else {
            static_assert(false, "Unsupported uniform type");
        }
    }, uniform.value);
}

void OpenGLShader::Clear() noexcept
{
    if (m_vertex_shader) {
        glDeleteShader(m_vertex_shader);
    }

    if (m_fragment_shader) {
        glDeleteShader(m_fragment_shader);
    }

    if (m_shader_program) {
        glDeleteProgram(m_shader_program);
    }

    m_vertex_shader   = 0;
    m_fragment_shader = 0;
    m_shader_program  = 0;

    m_uniform_cache.clear();
}

bool OpenGLShader::IsValid() const noexcept
{
    return m_shader_program != 0;
}

void OpenGLShader::BindAttributeLocation(std::uint32_t location, const std::string& name) const
{
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glBindAttribLocation(m_shader_program, location, name.c_str());
}

int OpenGLShader::GetAttributeLocation(const std::string& name) const
{
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return -1;
    }
    return glGetAttribLocation(m_shader_program, name.c_str());
}

int OpenGLShader::GetUniformLocation(const std::string& name) const
{
    if (m_uniform_cache.find(name) != m_uniform_cache.end()) {
        return m_uniform_cache[name];
    }

    int location = glGetUniformLocation(m_shader_program, name.c_str());
    if (location == -1) {
        LOG_ERROR << "[SHADER] Uniform '" << name << "' not found." << std::endl;
        return location;
    }

    m_uniform_cache[name] = location;
    return location;
}

bool OpenGLShader::LinkImpl()
{
    if (m_vertex_source.empty() || m_fragment_source.empty()) {
        return false;
    }

    if (m_vertex_shader == 0) {
        m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    }

    if (m_fragment_shader == 0) {
        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (!LoadShader(m_vertex_shader, GL_VERTEX_SHADER, m_vertex_source)) {
        Clear();
        return false;
    }

    if (!LoadShader(m_fragment_shader, GL_FRAGMENT_SHADER, m_fragment_source)) {
        Clear();
        return false;
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    if (m_shader_program == 0) {
        m_shader_program = glCreateProgram();
    }

    if (!CompileShaderProgram(m_shader_program, m_vertex_shader, m_fragment_shader)) {
        Clear();
        return false;
    }

    return true;
}

void swap(OpenGLShader& a, OpenGLShader& b) noexcept
{
    using std::swap;

    swap(a.m_renderer, b.m_renderer);
    swap(a.m_vertex_source, b.m_vertex_source);
    swap(a.m_fragment_source, b.m_fragment_source);
    swap(a.m_vertex_shader, b.m_vertex_shader);
    swap(a.m_fragment_shader, b.m_fragment_shader);
    swap(a.m_shader_program, b.m_shader_program);
    swap(a.m_uniform_cache, b.m_uniform_cache);
}

} // namespace game_engine::graphics
