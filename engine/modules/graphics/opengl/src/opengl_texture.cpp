
#include "opengl_texture.hpp"

#include <stdexcept>

#include <opengl_utils.hpp>

namespace game_engine::graphics
{

OpenGLTexture::OpenGLTexture()  = default;
OpenGLTexture::~OpenGLTexture() = default;

OpenGLTexture::OpenGLTexture(OpenGLTexture&& other) noexcept
    : OpenGLTexture()
{
    swap(*this, other);
}

OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& other) noexcept
{
    OpenGLTexture tmp(std::move(other));
    swap(*this, other);
    return *this;
}

bool OpenGLTexture::Load(const std::shared_ptr<ITexture>& texture)
{
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->GetWidth(), texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

bool OpenGLTexture::IsValid() const noexcept
{
    return false;
}

void OpenGLTexture::Clear() noexcept
{}

void swap(OpenGLTexture& a, OpenGLTexture& b) noexcept
{
    using std::swap;
}

} // namespace game_engine::graphics
