#include "opengl_utils.hpp"

#include <glad/glad.h>

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine::graphics
{

std::string_view GetErrorName(unsigned int error)
{
    switch (error) {
        case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
    }

    return "UNKNOWN_ERROR";
}

bool HasOpenGLErrors()
{
    bool has_errors = false;
    while (const GLuint error = glGetError()) {
        if (error == GL_NO_ERROR) {
            break;
        }

        LOG_ERROR << GetErrorName(error) << "\n";
        has_errors = true;
    }

    return has_errors;
}

} // namespace game_engine::graphics
