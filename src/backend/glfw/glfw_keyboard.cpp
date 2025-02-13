#include "glfw_keyboard.hpp"

#include <type_traits>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

KeyCode convert_glfw_key(int glfw_key) {
    using namespace game_engine;

    switch (glfw_key) {
        case GLFW_KEY_SPACE: return KeyCode::Space;
        case GLFW_KEY_ESCAPE:
            return KeyCode::Escape;

            // Add more keys

        default: return KeyCode::Unknown;
    }
}

KeyAction convert_glfw_action(int glfw_action) {
    using namespace game_engine;

    switch (glfw_action) {
        case GLFW_PRESS:   return KeyAction::Press;
        case GLFW_RELEASE: return KeyAction::Release;
        case GLFW_REPEAT:  return KeyAction::Repeat;
        default:           return KeyAction::Press;
    }
}

KeyModifier convert_glfw_modifiers(int glfw_mods) {
    using namespace game_engine;

    using UnderlyingType     = std::underlying_type_t<KeyModifier>;
    UnderlyingType modifiers = static_cast<UnderlyingType>(KeyModifier::None);

    if (glfw_mods & GLFW_MOD_SHIFT) {
        modifiers |= static_cast<UnderlyingType>(KeyModifier::Shift);
    }
    if (glfw_mods & GLFW_MOD_CONTROL) {
        modifiers |= static_cast<UnderlyingType>(KeyModifier::Control);
    }
    if (glfw_mods & GLFW_MOD_ALT) {
        modifiers |= static_cast<UnderlyingType>(KeyModifier::Alt);
    }
    if (glfw_mods & GLFW_MOD_SUPER) {
        modifiers |= static_cast<UnderlyingType>(KeyModifier::Super);
    }

    return static_cast<KeyModifier>(modifiers);
}

} // namespace game_engine::backend
