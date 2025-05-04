#include "opengl_mesh.hpp"

#include <stdexcept>

#include <glad/glad.h>
#include <opengl_utils.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

// TODO: move all openGL stuff in render thread
namespace
{

GLenum ToGLType(game_engine::core::VertexAttributeType type)
{
    switch (type) {
        case game_engine::core::VertexAttributeType::Float: return GL_FLOAT;
        case game_engine::core::VertexAttributeType::Int:   return GL_INT;
        case game_engine::core::VertexAttributeType::UInt:  return GL_UNSIGNED_INT;

        default: throw std::runtime_error("Unsupported type.");
    }
}

GLenum ToGLPrimitiveType(game_engine::core::PrimitiveType primitiveType)
{
    switch (primitiveType) {
        case game_engine::core::PrimitiveType::Triangles:     return GL_TRIANGLES;
        case game_engine::core::PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
        case game_engine::core::PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
        case game_engine::core::PrimitiveType::Lines:         return GL_LINES;
        case game_engine::core::PrimitiveType::LineStrip:     return GL_LINE_STRIP;
        case game_engine::core::PrimitiveType::LineLoop:      return GL_LINE_LOOP;
        case game_engine::core::PrimitiveType::Points:        return GL_POINTS;

        default: throw std::runtime_error("Unsupported primitive type.");
    }
}

} // namespace

namespace game_engine::renderer
{

OpenGLMesh::OpenGLMesh(std::shared_ptr<OpenGLRenderer> renderThread) noexcept
    : m_renderer(renderThread)
{}

OpenGLMesh::~OpenGLMesh()
{
    clear();
}

OpenGLMesh::OpenGLMesh(OpenGLMesh&& other) noexcept
{
    swap(*this, other);
}

OpenGLMesh& OpenGLMesh::operator=(OpenGLMesh&& other) noexcept
{
    auto tmp = std::move(other);
    swap(*this, tmp);

    return *this;
}

#pragma region core::Mesh

void OpenGLMesh::setMeshData(const core::MeshData& data)
{
    m_data = data;
}

void OpenGLMesh::flush()
{
    auto result = m_renderer->submitSync([this]() {
        if (!loadToGPU()) {
            // TODO: report error
        }
    });

    try {
        result.get();
    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
    }
}

void OpenGLMesh::clear() noexcept
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    m_VAO = 0;
    m_VBO = 0;
    m_EBO = 0;
}

bool OpenGLMesh::isValid() const noexcept
{
    return m_VAO != 0 && m_VBO != 0 && m_EBO != 0;
}

#pragma endregion

#pragma region OpenGLMesh

void OpenGLMesh::render() const
{
    if (m_data.submeshes.empty()) {
        return;
    }

    glBindVertexArray(m_VAO);

    const GLsizei indicesCount = static_cast<GLsizei>(m_data.submeshes[0].indices.size());
    if (indicesCount == 0) {
        return;
    }

    const GLenum primitiveType = ToGLPrimitiveType(m_data.primitiveType);
    glDrawElements(primitiveType, indicesCount, GL_UNSIGNED_INT, 0);
}

#pragma endregion

#pragma region OpenGLMesh private

bool OpenGLMesh::loadToGPU()
{
    // Create buffers
    if (m_VAO == 0) {
        glGenVertexArrays(1, &m_VAO);
    }

    if (m_VBO == 0) {
        glGenBuffers(1, &m_VBO);
    }

    if (m_EBO == 0) {
        glGenBuffers(1, &m_EBO);
    }

    glBindVertexArray(m_VAO);

    // Load data
    {
        const GLsizeiptr dataSize = m_data.vertexCount * m_data.layout.vertexSize;
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_data.vertexData.data());
    }

    if (hasOpenGLErrors()) {
        clear();
        return false;
    }

    // Load indices
    // TODO: Implement instancing
    if (!m_data.submeshes.empty() && !m_data.submeshes[0].indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        const GLsizeiptr dataSize = m_data.submeshes[0].indices.size() * sizeof(m_data.submeshes[0].indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dataSize, m_data.vertexData.data());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, m_data.submeshes[0].indices.data(), GL_STATIC_DRAW);
    }

    if (hasOpenGLErrors()) {
        clear();
        return false;
    }

    // Setup attributes
    for (const auto& attr : m_data.layout.attributes) {
        const GLenum type = ToGLType(attr.type);

        if (type == GL_INT || type == GL_UNSIGNED_INT) {
            glVertexAttribIPointer(attr.location,
                                   attr.components,
                                   type,
                                   static_cast<GLsizei>(m_data.layout.vertexSize),
                                   reinterpret_cast<void*>(attr.offset));
        } else if (type == GL_FLOAT) {
            glVertexAttribPointer(attr.location,
                                  attr.components,
                                  type,
                                  attr.normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(m_data.layout.vertexSize),
                                  reinterpret_cast<void*>(attr.offset));
        }

        glEnableVertexAttribArray(attr.location);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (hasOpenGLErrors()) {
        clear();
        return false;
    }

    return true;
}

void swap(OpenGLMesh& a, OpenGLMesh& b)
{
    using std::swap;

    swap(a.m_renderer, b.m_renderer);
    swap(a.m_VAO, b.m_VAO);
    swap(a.m_VBO, b.m_VBO);
    swap(a.m_EBO, b.m_EBO);
    swap(a.m_data, b.m_data);
}

#pragma endregion

} // namespace game_engine::renderer
