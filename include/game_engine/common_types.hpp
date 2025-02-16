
#pragma once

#include <glm/glm.hpp>

namespace game_engine
{
// Vectors
using Vector2 = glm::vec2; // 2D vector
using Vector3 = glm::vec3; // 3D vector
using Vector4 = glm::vec4; // 4D vector

// Integer vectors
using IVector2 = glm::ivec2; // 2D vector (int)
using IVector3 = glm::ivec3; // 3D vector (int)
using IVector4 = glm::ivec4; // 4D vector (int)

// Unsigned integer vectors
using UVector2 = glm::uvec2; // 2D vector (unsigned int)
using UVector3 = glm::uvec3; // 3D vector (unsigned int)
using UVector4 = glm::uvec4; // 4D vector (unsigned int)

// Matrices
using Matrix2 = glm::mat2; // 2x2 matrix
using Matrix3 = glm::mat3; // 3x3 matrix
using Matrix4 = glm::mat4; // 4x4 matrix

// Non-square matrices (if needed)
using Matrix2x3 = glm::mat2x3; // 2x3 matrix
using Matrix3x2 = glm::mat3x2; // 3x2 matrix
using Matrix4x3 = glm::mat4x3; // 4x3 matrix

// Quaternions
using Quaternion = glm::quat; // Quaternion for rotations

} // namespace game_engine
