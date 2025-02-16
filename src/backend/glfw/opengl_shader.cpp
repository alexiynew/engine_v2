#include "opengl_shader.hpp"

#include <iostream>
#include <memory>
#include <string_view>

#include <glad/glad.h>

#define LOG_ERROR std::cerr

namespace
{
std::string_view get_error_name(GLuint error) {
    switch (error) {
        case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
    }

    return "UNKNOWN_ERROR";
}

bool has_opengl_errors() {
    bool has_errors = false;
    while (GLuint error = glGetError()) {
        if (error == GL_NO_ERROR) {
            break;
        }

        LOG_ERROR << get_error_name(error) << "\n";
        has_errors = true;
    }

    return has_errors;
}

std::string shader_type_string(int shader_type) {
    switch (shader_type) {
        case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
    }

    return "UNKNOWN_SHADER";
}

std::string shader_info_log(GLuint shader_id) {
    int length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return std::string();
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetShaderInfoLog(shader_id, length, nullptr, buffer.get());

    return std::string(buffer.get());
}

std::string shader_program_info_log(GLuint program_id) {
    int length = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return std::string();
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetProgramInfoLog(program_id, length, nullptr, buffer.get());

    return std::string(buffer.get());
}

bool load_shader(GLuint shader_id, int shader_type, const std::string& source) {
    if (shader_id == 0) {
        return false;
    }

    const char* source_pointer = source.c_str();

    glShaderSource(shader_id, 1, &source_pointer, nullptr);
    glCompileShader(shader_id);

    int compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        LOG_ERROR << shader_type_string(shader_type) << " compilation error:\n"
                  << shader_info_log(shader_id) << std::endl;
        return false;
    }

    return true;
}

bool compile_shader_program(GLuint program_id, GLuint vertex_shader_id, GLuint fragment_shader_id) {
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
        LOG_ERROR << "Shader program link error:\n" << shader_program_info_log(program_id) << std::endl;
        return false;
    }

    return true;
}

} // namespace

namespace game_engine::backend
{

OpenGLShader::OpenGLShader() noexcept = default;

OpenGLShader::~OpenGLShader() noexcept {
    clear();
}

OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept {
    swap(*this, other);
}

OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept {
    OpenGLShader tmp = std::move(other);
    swap(*this, tmp);

    return *this;
}

bool OpenGLShader::link(const std::string& vertex_source, const std::string& fragment_source) {
    if (m_vertex_shader == 0) {
        m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    }

    if (m_fragment_shader == 0) {
        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (!load_shader(m_vertex_shader, GL_VERTEX_SHADER, vertex_source)) {
        clear();
        return false;
    }

    if (!load_shader(m_fragment_shader, GL_FRAGMENT_SHADER, fragment_source)) {
        clear();
        return false;
    }

    if (has_opengl_errors()) {
        clear();
        return false;
    }

    if (m_shader_program == 0) {
        m_shader_program = glCreateProgram();
    }

    if (!compile_shader_program(m_shader_program, m_vertex_shader, m_fragment_shader)) {
        clear();
        return false;
    }

    return true;
}

void OpenGLShader::clear() {
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

void OpenGLShader::use() const {
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glUseProgram(m_shader_program);
}

int OpenGLShader::get_uniform_location(const std::string& name) const {
    if (m_uniform_cache.find(name) != m_uniform_cache.end()) {
        return m_uniform_cache[name];
    }

    int location = glGetUniformLocation(m_shader_program, name.c_str());
    if (location == -1) {
        LOG_ERROR << "[SHADER] Uniform '" << name << "' not found." << std::endl;
    }

    m_uniform_cache[name] = location;
    return location;
}

void OpenGLShader::set_uniform(int uniform, std::uint32_t value) const {
    if (uniform < 0 || m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniform1i(uniform, value);
}

void OpenGLShader::set_uniform(int uniform, float value) const {
    if (uniform < 0 || m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniform1f(uniform, value);
}

void OpenGLShader::set_uniform(int uniform, const Matrix4& matrix) const {
    if (uniform < 0 || m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &matrix[0][0]);
}

void OpenGLShader::bind_attribute_location(std::uint32_t location, const std::string& name) const {
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glBindAttribLocation(m_shader_program, location, name.c_str());
}

int OpenGLShader::get_attribute_location(const std::string& name) const {
    if (m_shader_program == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return -1;
    }
    return glGetAttribLocation(m_shader_program, name.c_str());
}

bool OpenGLShader::is_valid() const {
    return m_shader_program != 0;
}

void swap(OpenGLShader& a, OpenGLShader& b) {
    using std::swap;

    swap(a.m_vertex_shader, b.m_vertex_shader);
    swap(a.m_fragment_shader, b.m_fragment_shader);
    swap(a.m_shader_program, b.m_shader_program);
    swap(a.m_uniform_cache, b.m_uniform_cache);
}

} // namespace game_engine::backend
