#pragma once

#include <string>
#include <variant>

#include <game_engine/common_types.hpp>

namespace game_engine::core
{

/// @brief Type alias for a variant that can hold various types of uniform values.
using Uniform = std::variant<int, float, Vector2, Vector3, Vector4, Matrix3, Matrix4>;

/// @brief Base class for managing shader programs.
///
/// Provides an interface for setting shader source code, linking shader programs,
/// setting uniform variables, and managing the lifecycle of the shader.
class Shader
{
public:
    virtual ~Shader() = default;

    /// @brief Sets the source code for the vertex and fragment shaders.
    /// @param vertexSource The source code for the vertex shader.
    /// @param fragmentSource The source code for the fragment shader.
    virtual void setSource(const std::string& vertexSource, const std::string& fragmentSource) = 0;

    /// @brief Links the shader program.
    /// This method compiles the vertex and fragment shaders and links them into a single shader program.
    /// @return True if the shader program was successfully linked, false otherwise.
    virtual bool link() = 0;

    /// @brief Activates the shader program for rendering.
    virtual void use() const = 0;

    /// @brief Sets a uniform variable in the shader program.
    /// @param name The name of the uniform variable in the shader.
    /// @param uniform The value to set for the uniform variable.
    virtual void setUniform(const std::string& name, const Uniform& uniform) = 0;

    /// @brief Clears the shader program by removing it from the context.
    virtual void clear() = 0;

    /// @brief Checks if the shader program is valid and ready to be used.
    /// A shader program is considered valid if it has been successfully linked.
    /// @return True if the shader program is valid, false otherwise.
    virtual bool isValid() const = 0;
};

} // namespace game_engine::core
