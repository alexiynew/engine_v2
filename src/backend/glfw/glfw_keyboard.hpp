#pragma once

#include <game_engine/keyboard.hpp>

namespace game_engine::backend
{

KeyCode convert_glfw_key(int glfw_key);
KeyAction convert_glfw_action(int glfw_action);
KeyModifier convert_glfw_modifiers(int glfw_mods);

} // namespace game_engine::backend
