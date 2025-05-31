#pragma once

#include <engine/keyboard.hpp>

namespace game_engine::backend
{

KeyCode ConvertGlfwKey(int glfw_key);
KeyAction ConvertGlfwAction(int glfw_action);
KeyModifier ConvertGlfwModifiers(int glfw_mods);

} // namespace game_engine::backend
