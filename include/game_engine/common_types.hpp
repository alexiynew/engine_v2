#pragma once

#include <glm/glm.hpp>

namespace game_engine
{
/// Vectors
using Vector2 = glm::vec2; ///< 2D vector
using Vector3 = glm::vec3; ///< 3D vector
using Vector4 = glm::vec4; ///< 4D vector

/// Integer vectors
using Vector2i = glm::ivec2; ///< 2D vector (int)
using Vector3i = glm::ivec3; ///< 3D vector (int)
using Vector4i = glm::ivec4; ///< 4D vector (int)

/// Unsigned integer vectors
using Vector2u = glm::uvec2; ///< 2D vector (unsigned int)
using Vector3u = glm::uvec3; ///< 3D vector (unsigned int)
using Vector4u = glm::uvec4; ///< 4D vector (unsigned int)

/// Matrices
using Matrix2 = glm::mat2; ///< 2x2 matrix
using Matrix3 = glm::mat3; ///< 3x3 matrix
using Matrix4 = glm::mat4; ///< 4x4 matrix

/// Non-square matrices
using Matrix2x3 = glm::mat2x3; ///< 2x3 matrix
using Matrix3x2 = glm::mat3x2; ///< 3x2 matrix
using Matrix4x3 = glm::mat4x3; ///< 4x3 matrix

/// Quaternions
using Quaternion = glm::quat; ///< Quaternion for rotations

} // namespace game_engine
