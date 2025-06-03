#include "opengl_mesh.hpp"

#include <stdexcept>

#include <glad/glad.h>
#include <opengl_utils.hpp>

namespace
{

GLenum ToGLType(game_engine::VertexAttributeType type)
{
    switch (type) {
        case game_engine::VertexAttributeType::Float: return GL_FLOAT;
        case game_engine::VertexAttributeType::Int:   return GL_INT;
        case game_engine::VertexAttributeType::UInt:  return GL_UNSIGNED_INT;

        default: throw std::runtime_error("Unsupported type.");
    }
}

GLenum ToGLPrimitiveType(game_engine::PrimitiveType primitive_type)
{
    switch (primitive_type) {
        case game_engine::PrimitiveType::Triangles:     return GL_TRIANGLES;
        case game_engine::PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
        case game_engine::PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
        case game_engine::PrimitiveType::Lines:         return GL_LINES;
        case game_engine::PrimitiveType::LineStrip:     return GL_LINE_STRIP;
        case game_engine::PrimitiveType::LineLoop:      return GL_LINE_LOOP;
        case game_engine::PrimitiveType::Points:        return GL_POINTS;

        default: throw std::runtime_error("Unsupported primitive type.");
    }
}

} // namespace

namespace game_engine::graphics
{

OpenGLMesh::OpenGLMesh() = default;

OpenGLMesh::~OpenGLMesh()
{
    Clear();
}

OpenGLMesh::OpenGLMesh(OpenGLMesh&& other) noexcept
    : OpenGLMesh()
{
    swap(*this, other);
}

OpenGLMesh& OpenGLMesh::operator=(OpenGLMesh&& other) noexcept
{
    OpenGLMesh tmp(std::move(other));
    swap(*this, other);
    return *this;
}

bool OpenGLMesh::Load(const std::shared_ptr<IMesh>& mesh)
{
    // Create buffers
    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }

    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }

    if (m_ebo == 0) {
        glGenBuffers(1, &m_ebo);
    }

    glBindVertexArray(m_vao);

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Load data
    {
        const auto& data = mesh->GetVertexData();

        const GLsizeiptr data_size = data.vertex_count * data.vertex_size;

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, data_size, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data.data.data());
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Load indices
    {
        // TODO: Implement instancing
        const auto& data      = mesh->GetVertexData();
        const auto& submeshes = mesh->GetSubMeshes();

        if (!submeshes.empty() && !submeshes[0].indices.empty()) {
            const GLsizeiptr data_size = submeshes[0].indices.size() * sizeof(submeshes[0].indices[0]);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, submeshes[0].indices.data());
        }

        m_indices_count = static_cast<int>(submeshes[0].indices.size());
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Setup attributes
    {
        const auto& data = mesh->GetVertexData();

        for (const auto& attr : data.attributes) {
            const GLenum type = ToGLType(attr.type);

            if (type == GL_INT || type == GL_UNSIGNED_INT) {
                glVertexAttribIPointer(attr.location,
                attr.components,
                type,
                static_cast<GLsizei>(data.vertex_size),
                reinterpret_cast<void*>(attr.offset));
            } else if (type == GL_FLOAT) {
                glVertexAttribPointer(attr.location,
                attr.components,
                type,
                attr.normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(data.vertex_size),
                reinterpret_cast<void*>(attr.offset));
            }

            glEnableVertexAttribArray(attr.location);
        }
    }

    m_primitive_type = ToGLPrimitiveType(mesh->GetPrimitiveType());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    return true;
}

bool OpenGLMesh::IsValid() const noexcept
{
    return m_vao != 0 && m_vbo != 0 && m_ebo != 0 && m_indices_count > 0 && m_primitive_type != 0;
}

void OpenGLMesh::Clear() noexcept
{
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }

    if (m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
    }

    m_primitive_type = 0;

    m_indices_count = 0;

    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
}

void OpenGLMesh::Render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(m_primitive_type, m_indices_count, GL_UNSIGNED_INT, 0);
}

void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept
{
    using std::swap;

    swap(a.m_primitive_type, b.m_primitive_type);
    swap(a.m_indices_count, b.m_indices_count);
    swap(a.m_vao, b.m_vao);
    swap(a.m_vbo, b.m_vbo);
    swap(a.m_ebo, b.m_ebo);
}

} // namespace game_engine::graphics
