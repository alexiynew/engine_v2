
#include "opengl_texture.hpp"

#include <stdexcept>

#include <glad/glad.h>
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
    return false;
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
