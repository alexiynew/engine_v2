#include "opengl_shader.hpp"

#include <iostream>
#include <memory>
#include <string_view>

#include <glad/glad.h>
#include <glfw/opengl_utils.hpp>

#define LOG_ERROR std::cerr

namespace
{
std::string shaderTypeString(int shaderType)
{
    switch (shaderType) {
        case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
    }

    return "UNKNOWN_SHADER";
}

std::string shaderInfoLog(GLuint shaderId)
{
    int length = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return std::string();
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetShaderInfoLog(shaderId, length, nullptr, buffer.get());

    return std::string(buffer.get());
}

std::string shaderProgramInfoLog(GLuint programId)
{
    int length = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0) {
        return std::string();
    }

    std::unique_ptr<char[]> buffer(new char[static_cast<std::size_t>(length)]);
    glGetProgramInfoLog(programId, length, nullptr, buffer.get());

    return std::string(buffer.get());
}

bool loadShader(GLuint shaderId, int shaderType, const std::string& source)
{
    if (shaderId == 0) {
        return false;
    }

    const char* sourcePointer = source.c_str();

    glShaderSource(shaderId, 1, &sourcePointer, nullptr);
    glCompileShader(shaderId);

    int compiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        LOG_ERROR << shaderTypeString(shaderType) << " compilation error:\n" << shaderInfoLog(shaderId) << std::endl;
        return false;
    }

    return true;
}

bool compileShaderProgram(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId)
{
    if (programId == 0 || vertexShaderId == 0 || fragmentShaderId == 0) {
        return false;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);

    // mark shader for deletion
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    int linked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    if (linked == 0) {
        LOG_ERROR << "Shader program link error:\n" << shaderProgramInfoLog(programId) << std::endl;
        return false;
    }

    return true;
}

} // namespace

namespace game_engine::backend
{

OpenGLShader::OpenGLShader() noexcept = default;

OpenGLShader::~OpenGLShader() noexcept
{
    clear();
}

OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
{
    swap(*this, other);
}

OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept
{
    OpenGLShader tmp = std::move(other);
    swap(*this, tmp);

    return *this;
}

bool OpenGLShader::link(const std::string& vertexSource, const std::string& fragmentSource)
{
    if (m_vertexShader == 0) {
        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    }

    if (m_fragmentShader == 0) {
        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (!loadShader(m_vertexShader, GL_VERTEX_SHADER, vertexSource)) {
        clear();
        return false;
    }

    if (!loadShader(m_fragmentShader, GL_FRAGMENT_SHADER, fragmentSource)) {
        clear();
        return false;
    }

    if (hasOpenGLErrors()) {
        clear();
        return false;
    }

    if (m_shaderProgram == 0) {
        m_shaderProgram = glCreateProgram();
    }

    if (!compileShaderProgram(m_shaderProgram, m_vertexShader, m_fragmentShader)) {
        clear();
        return false;
    }

    return true;
}

void OpenGLShader::clear()
{
    if (m_vertexShader) {
        glDeleteShader(m_vertexShader);
    }

    if (m_fragmentShader) {
        glDeleteShader(m_fragmentShader);
    }

    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
    }

    m_vertexShader   = 0;
    m_fragmentShader = 0;
    m_shaderProgram  = 0;

    m_uniformCache.clear();
}

void OpenGLShader::use() const
{
    if (m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glUseProgram(m_shaderProgram);
}

int OpenGLShader::getUniformLocation(const std::string& name) const
{
    if (m_uniformCache.find(name) != m_uniformCache.end()) {
        return m_uniformCache[name];
    }

    int location = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (location == -1) {
        LOG_ERROR << "[SHADER] Uniform '" << name << "' not found." << std::endl;
    }

    m_uniformCache[name] = location;
    return location;
}

void OpenGLShader::setUniform(int uniform, std::uint32_t value) const
{
    if (uniform < 0 || m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniform1i(uniform, static_cast<GLint>(value));
}

void OpenGLShader::setUniform(int uniform, float value) const
{
    if (uniform < 0 || m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniform1f(uniform, value);
}

void OpenGLShader::setUniform(int uniform, const Matrix4& matrix) const
{
    if (uniform < 0 || m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Uniform is negative or shader program is not initialized." << std::endl;
        return;
    }
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &matrix[0][0]);
}

void OpenGLShader::bindAttributeLocation(std::uint32_t location, const std::string& name) const
{
    if (m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return;
    }
    glBindAttribLocation(m_shaderProgram, location, name.c_str());
}

int OpenGLShader::getAttributeLocation(const std::string& name) const
{
    if (m_shaderProgram == 0) {
        LOG_ERROR << "[SHADER] Shader program is not initialized." << std::endl;
        return -1;
    }
    return glGetAttribLocation(m_shaderProgram, name.c_str());
}

bool OpenGLShader::isValid() const
{
    return m_shaderProgram != 0;
}

void swap(OpenGLShader& a, OpenGLShader& b)
{
    using std::swap;

    swap(a.m_vertexShader, b.m_vertexShader);
    swap(a.m_fragmentShader, b.m_fragmentShader);
    swap(a.m_shaderProgram, b.m_shaderProgram);
    swap(a.m_uniformCache, b.m_uniformCache);
}

} // namespace game_engine::backend
