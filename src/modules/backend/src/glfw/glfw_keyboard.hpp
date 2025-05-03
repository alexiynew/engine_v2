#pragma once

#include <game_engine/keyboard.hpp>

namespace game_engine::backend
{

KeyCode convertGLFWKey(int glfwKey);
KeyAction convertGLFWAction(int glfwAction);
KeyModifier convertGLFWModifiers(int glfwMods);

} // namespace game_engine::backend
