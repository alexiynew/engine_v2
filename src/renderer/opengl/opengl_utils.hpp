#pragma once

#include <string_view>

namespace game_engine::renderer
{

std::string_view getErrorName(unsigned int error);

bool hasOpenGLErrors();

} // namespace game_engine::renderer
