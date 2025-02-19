#include "opengl_utils.hpp"

#include <iostream>

#include <glad/glad.h>

#define LOG_ERROR std::cerr

namespace game_engine::backend
{

std::string_view getErrorName(unsigned int error) {
    switch (error) {
        case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
    }

    return "UNKNOWN_ERROR";
}

bool hasOpenGLErrors() {
    bool hasErrors = false;
    while (GLuint error = glGetError()) {
        if (error == GL_NO_ERROR) {
            break;
        }

        LOG_ERROR << getErrorName(error) << "\n";
        hasErrors = true;
    }

    return hasErrors;
}

} // namespace game_engine::backend
