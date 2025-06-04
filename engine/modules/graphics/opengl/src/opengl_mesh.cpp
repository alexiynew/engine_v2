#include "opengl_mesh.hpp"

#include <numeric>
#include <stdexcept>

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

GLsizeiptr GetIndicesDataSize(const std::vector<game_engine::SubMesh>& submeshes)
{
    const auto size = std::accumulate(submeshes.begin(), submeshes.end(), 0, [](std::size_t size_sum, const auto& submesh) {
        return size_sum + submesh.indices.size() * sizeof(submesh.indices[0]);
    });

    return static_cast<GLsizeiptr>(size);
}

template <typename T>
GLenum GetIndicesType(const std::vector<T>&)
{
    if constexpr (std::is_same_v<T, unsigned int>) {
        return GL_UNSIGNED_INT;
    } else if constexpr (std::is_same_v<T, unsigned short>) {
        return GL_UNSIGNED_SHORT;
    } else if constexpr (std::is_same_v<T, unsigned char>) {
        return GL_UNSIGNED_BYTE;
    } else {
        static_assert(!std::is_same_v<T, T>, "Unsupported indices type.");
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

        m_vertex_count = static_cast<GLint>(data.vertex_count);
    }

    if (HasOpenGLErrors()) {
        Clear();
        return false;
    }

    // Load indices
    {
        // TODO: Implement instancing
        const GLsizeiptr index_data_size = GetIndicesDataSize(mesh->GetSubMeshes());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, nullptr, GL_STATIC_DRAW);

        m_submeshes.clear();

        GLintptr current_offset = 0;
        for (const auto& submesh : mesh->GetSubMeshes()) {
            if (!submesh.indices.empty()) {
                const GLsizeiptr size   = submesh.indices.size() * sizeof(submesh.indices[0]);
                const GLenum index_type = GetIndicesType(submesh.indices);

                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, current_offset, size, submesh.indices.data());
                m_submeshes.push_back(
                    {.count = static_cast<GLsizei>(submesh.indices.size()), .offset = current_offset, .type = index_type});
                current_offset += size;
            }
        }
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
    return m_vao != 0 && m_vbo != 0 && m_ebo != 0 && m_vertex_count > 0 && m_primitive_type != 0;
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

    m_submeshes.clear();

    m_vertex_count   = 0;
    m_primitive_type = 0;

    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
}

void OpenGLMesh::Render() const
{
    glBindVertexArray(m_vao);

    if (!m_submeshes.empty()) {
        for (const auto& submesh : m_submeshes) {
            glDrawElements(m_primitive_type, submesh.count, submesh.type, reinterpret_cast<void*>(submesh.offset));
        }
    } else {
        glDrawArrays(m_primitive_type, 0, m_vertex_count);
    }
}

void OpenGLMesh::RenderInstanced(GLsizei instance_count) const
{
    if (!m_submeshes.empty()) {
        for (const auto& submesh : m_submeshes) {
            glDrawElementsInstanced(m_primitive_type, submesh.count, submesh.type, reinterpret_cast<void*>(submesh.offset), instance_count);
        }
    } else {
        glDrawArraysInstanced(m_primitive_type, 0, m_vertex_count, instance_count);
    }
}

void swap(OpenGLMesh& a, OpenGLMesh& b) noexcept
{
    using std::swap;

    swap(a.m_submeshes, b.m_submeshes);
    swap(a.m_vertex_count, b.m_vertex_count);
    swap(a.m_primitive_type, b.m_primitive_type);
    swap(a.m_vao, b.m_vao);
    swap(a.m_vbo, b.m_vbo);
    swap(a.m_ebo, b.m_ebo);
}

} // namespace game_engine::graphics
