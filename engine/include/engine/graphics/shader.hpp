#pragma once

#include <string>
#include <variant>

#include <engine/common_types.hpp>

namespace game_engine::graphics
{

/// @brief Type alias for a variant that can hold various types of uniform values.
using UniformValue = std::variant<int, float, Vector2, Vector3, Vector4, Matrix3, Matrix4>;

struct Uniform
{
    std::string name;
    UniformValue value;
};

/// @brief Base class for managing shader programs.
///
/// Provides an interface for setting shader source code, linking shader programs,
/// setting uniform variables, and managing the lifecycle of the shader.
class IShader
{
public:
    virtual ~IShader() = default;

    /// @brief Sets the source code for the vertex and fragment shaders.
    /// @param vertex_source The source code for the vertex shader.
    /// @param fragment_source The source code for the fragment shader.
    virtual void SetSource(const std::string& vertex_source, const std::string& fragment_source) = 0;

    /// @brief Links the shader program.
    /// This method compiles the vertex and fragment shaders and links them into a single shader program.
    /// @return True if the shader program was successfully linked, false otherwise.
    virtual bool Link() = 0;

    /// @brief Clears the shader program by removing it from the context.
    virtual void Clear() noexcept = 0;

    /// @brief Checks if the shader program is valid and ready to be used.
    /// A shader program is considered valid if it has been successfully linked.
    /// @return True if the shader program is valid, false otherwise.
    virtual bool IsValid() const noexcept = 0;
};

} // namespace game_engine::graphics
