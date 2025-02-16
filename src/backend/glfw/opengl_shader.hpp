#pragma once

#include <string>
#include <unordered_map>

#include <game_engine/common_types.hpp>

namespace game_engine::backend
{

/**
 * @class OpenGLShader
 * @brief A wrapper class for managing OpenGL shaders and shader programs.
 *
 * This class provides an interface to compile, link, and use OpenGL shaders.
 * It supports vertex and fragment shaders, and allows setting uniforms and attributes.
 */
class OpenGLShader
{
public:
    OpenGLShader() noexcept;
    ~OpenGLShader() noexcept;

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader(OpenGLShader&& other) noexcept;

    OpenGLShader& operator=(const OpenGLShader&) = delete;
    OpenGLShader& operator=(OpenGLShader&& other) noexcept;

    /**
     * @brief Compiles and links the shader program.
     * @param vertex_source The source code of the vertex shader.
     * @param fragment_source The source code of the fragment shader.
     * @return True if the shader program was successfully compiled and linked, false otherwise.
     */
    bool link(const std::string& vertex_source, const std::string& fragment_source);

    /**
     * @brief Clears all shader and program resources.
     */
    void clear();

    /**
     * @brief Uses the shader program for rendering.
     */
    void use() const;

    /**
     * @brief Gets the location of a uniform variable.
     * @param name The name of the uniform variable.
     * @return The location of the uniform variable, or -1 if not found.
     */
    int get_uniform_location(const std::string& name) const;

    /**
     * @brief Sets a uniform variable to an integer value.
     * @param uniform The location of the uniform variable.
     * @param value The integer value to set.
     */
    void set_uniform(int uniform, std::uint32_t value) const;

    /**
     * @brief Sets a uniform variable to a float value.
     * @param uniform The location of the uniform variable.
     * @param value The float value to set.
     */
    void set_uniform(int uniform, float value) const;

    /**
     * @brief Sets a uniform variable to a 4x4 matrix value.
     * @param uniform The location of the uniform variable.
     * @param matrix The 4x4 matrix value to set.
     */
    void set_uniform(int uniform, const Matrix4& matrix) const;

    /**
     * @brief Binds an attribute to a specific location.
     * @param location The location to bind the attribute to.
     * @param name The name of the attribute.
     */
    void bind_attribute_location(std::uint32_t location, const std::string& name) const;

    /**
     * @brief Gets the location of an attribute.
     * @param name The name of the attribute.
     * @return The location of the attribute, or -1 if not found.
     */
    int get_attribute_location(const std::string& name) const;

    /**
     * @brief Checks if the shader program is valid and ready to be used.
     * @return True if the shader program is valid (i.e., it has been successfully linked), false otherwise.
     */
    bool is_valid() const;

private:
    std::uint32_t m_shader_program  = 0;
    std::uint32_t m_vertex_shader   = 0;
    std::uint32_t m_fragment_shader = 0;

    mutable std::unordered_map<std::string, int> m_uniform_cache;

    friend void swap(OpenGLShader& a, OpenGLShader& b);
};

} // namespace game_engine::backend
