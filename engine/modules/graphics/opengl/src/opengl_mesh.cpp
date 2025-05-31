#include "opengl_mesh.hpp"

#include <stdexcept>

#include <glad/glad.h>
#include <opengl_utils.hpp>

#define LOG_ERROR std::cerr
#include <iostream>

// TODO: move all openGL stuff in render thread
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

OpenGLMesh::OpenGLMesh(std::shared_ptr<OpenGLRenderer> render_thread) noexcept
    : m_renderer(render_thread)
{}

OpenGLMesh::~OpenGLMesh()
{
    Clear();
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

#pragma region graphics::Mesh

void OpenGLMesh::SetMeshData(const MeshData& data)
{
    m_data = data;
}

bool OpenGLMesh::LoadToGpu()
{
    auto result = m_renderer->SubmitSync([this]() {
        if (!LoadToGpuImpl()) {
            // TODO: report error
        }
    });

    try {
        result.get();
    } catch (std::exception& e) {
        LOG_ERROR << "Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void OpenGLMesh::Clear() noexcept
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);

    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
}

bool OpenGLMesh::IsValid() const noexcept
{
    return m_vao != 0 && m_vbo != 0 && m_ebo != 0;
}

#pragma endregion

#pragma region OpenGLMesh

void OpenGLMesh::Render() const
{
    if (m_data.submeshes.empty()) {
        return;
    }

    glBindVertexArray(m_vao);

    const GLsizei indices_count = static_cast<GLsizei>(m_data.submeshes[0].indices.size());
    if (indices_count == 0) {
        return;
    }

    const GLenum primitive_type = ToGLPrimitiveType(m_data.primitive_type);
    glDrawElements(primitive_type, indices_count, GL_UNSIGNED_INT, 0);
}

#pragma endregion

#pragma region OpenGLMesh private

bool OpenGLMesh::LoadToGpuImpl()
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

    // Load data
    {
        const GLsizeiptr data_size = m_data.vertex_count * m_data.layout.vertex_size;
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, data_size, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, m_data.vertex_data.data());
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Load indices
    // TODO: Implement instancing
    if (!m_data.submeshes.empty() && !m_data.submeshes[0].indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        const GLsizeiptr data_size = m_data.submeshes[0].indices.size() * sizeof(m_data.submeshes[0].indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, m_data.vertex_data.data());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, m_data.submeshes[0].indices.data(), GL_STATIC_DRAW);
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Setup attributes
    for (const auto& attr : m_data.layout.attributes) {
        const GLenum type = ToGLType(attr.type);

        if (type == GL_INT || type == GL_UNSIGNED_INT) {
            glVertexAttribIPointer(attr.location,
                                   attr.components,
                                   type,
                                   static_cast<GLsizei>(m_data.layout.vertex_size),
                                   reinterpret_cast<void*>(attr.offset));
        } else if (type == GL_FLOAT) {
            glVertexAttribPointer(attr.location,
                                  attr.components,
                                  type,
                                  attr.normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLsizei>(m_data.layout.vertex_size),
                                  reinterpret_cast<void*>(attr.offset));
        }

        glEnableVertexAttribArray(attr.location);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    return true;
}

void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept
{
    using std::swap;

    swap(a.m_renderer, b.m_renderer);
    swap(a.m_vao, b.m_vao);
    swap(a.m_vbo, b.m_vbo);
    swap(a.m_ebo, b.m_ebo);
    swap(a.m_data, b.m_data);
}

#pragma endregion

} // namespace game_engine::graphics
