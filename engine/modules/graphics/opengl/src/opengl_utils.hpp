#pragma once

#include <string_view>

namespace game_engine::graphics
{

std::string_view GetErrorName(unsigned int error);

bool HasOpenGLErrors();

} // namespace game_engine::graphics
