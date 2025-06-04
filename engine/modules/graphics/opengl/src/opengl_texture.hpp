#pragma once

#include <engine/graphics/texture.hpp>

#include <glad/glad.h>

namespace game_engine::graphics
{

class OpenGLTexture final
{
public:

    OpenGLTexture();
    ~OpenGLTexture();

    OpenGLTexture(const OpenGLTexture&) = delete;
    OpenGLTexture(OpenGLTexture&& other) noexcept;

    OpenGLTexture& operator=(const OpenGLTexture&) = delete;
    OpenGLTexture& operator=(OpenGLTexture&& other) noexcept;

    bool Load(const std::shared_ptr<ITexture>& mesh);
    bool IsValid() const noexcept;
    void Clear() noexcept;

private:

    friend void swap(OpenGLTexture& a, OpenGLTexture& b) noexcept;

    GLuint m_texture_id = 0;
};

} // namespace game_engine::graphics
