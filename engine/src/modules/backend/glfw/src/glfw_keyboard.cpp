#include "glfw_keyboard.hpp"

#include <type_traits>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

KeyCode ConvertGlfwKey(int glfw_key)
{
    using namespace game_engine;

    switch (glfw_key) {
        // Printable keys
        case GLFW_KEY_SPACE:         return KeyCode::Space;
        case GLFW_KEY_APOSTROPHE:    return KeyCode::Apostrophe;
        case GLFW_KEY_COMMA:         return KeyCode::Comma;
        case GLFW_KEY_MINUS:         return KeyCode::Minus;
        case GLFW_KEY_PERIOD:        return KeyCode::Period;
        case GLFW_KEY_SLASH:         return KeyCode::Slash;
        case GLFW_KEY_0:             return KeyCode::Key0;
        case GLFW_KEY_1:             return KeyCode::Key1;
        case GLFW_KEY_2:             return KeyCode::Key2;
        case GLFW_KEY_3:             return KeyCode::Key3;
        case GLFW_KEY_4:             return KeyCode::Key4;
        case GLFW_KEY_5:             return KeyCode::Key5;
        case GLFW_KEY_6:             return KeyCode::Key6;
        case GLFW_KEY_7:             return KeyCode::Key7;
        case GLFW_KEY_8:             return KeyCode::Key8;
        case GLFW_KEY_9:             return KeyCode::Key9;
        case GLFW_KEY_SEMICOLON:     return KeyCode::Semicolon;
        case GLFW_KEY_EQUAL:         return KeyCode::Equal;
        case GLFW_KEY_A:             return KeyCode::A;
        case GLFW_KEY_B:             return KeyCode::B;
        case GLFW_KEY_C:             return KeyCode::C;
        case GLFW_KEY_D:             return KeyCode::D;
        case GLFW_KEY_E:             return KeyCode::E;
        case GLFW_KEY_F:             return KeyCode::F;
        case GLFW_KEY_G:             return KeyCode::G;
        case GLFW_KEY_H:             return KeyCode::H;
        case GLFW_KEY_I:             return KeyCode::I;
        case GLFW_KEY_J:             return KeyCode::J;
        case GLFW_KEY_K:             return KeyCode::K;
        case GLFW_KEY_L:             return KeyCode::L;
        case GLFW_KEY_M:             return KeyCode::M;
        case GLFW_KEY_N:             return KeyCode::N;
        case GLFW_KEY_O:             return KeyCode::O;
        case GLFW_KEY_P:             return KeyCode::P;
        case GLFW_KEY_Q:             return KeyCode::Q;
        case GLFW_KEY_R:             return KeyCode::R;
        case GLFW_KEY_S:             return KeyCode::S;
        case GLFW_KEY_T:             return KeyCode::T;
        case GLFW_KEY_U:             return KeyCode::U;
        case GLFW_KEY_V:             return KeyCode::V;
        case GLFW_KEY_W:             return KeyCode::W;
        case GLFW_KEY_X:             return KeyCode::X;
        case GLFW_KEY_Y:             return KeyCode::Y;
        case GLFW_KEY_Z:             return KeyCode::Z;
        case GLFW_KEY_LEFT_BRACKET:  return KeyCode::LeftBracket;
        case GLFW_KEY_BACKSLASH:     return KeyCode::Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return KeyCode::RightBracket;
        case GLFW_KEY_GRAVE_ACCENT:  return KeyCode::GraveAccent;
        case GLFW_KEY_WORLD_1:       return KeyCode::World1;
        case GLFW_KEY_WORLD_2:       return KeyCode::World2;

        // Function keys
        case GLFW_KEY_ESCAPE:       return KeyCode::Escape;
        case GLFW_KEY_ENTER:        return KeyCode::Enter;
        case GLFW_KEY_TAB:          return KeyCode::Tab;
        case GLFW_KEY_BACKSPACE:    return KeyCode::Backspace;
        case GLFW_KEY_INSERT:       return KeyCode::Insert;
        case GLFW_KEY_DELETE:       return KeyCode::Delete;
        case GLFW_KEY_RIGHT:        return KeyCode::Right;
        case GLFW_KEY_LEFT:         return KeyCode::Left;
        case GLFW_KEY_DOWN:         return KeyCode::Down;
        case GLFW_KEY_UP:           return KeyCode::Up;
        case GLFW_KEY_PAGE_UP:      return KeyCode::PageUp;
        case GLFW_KEY_PAGE_DOWN:    return KeyCode::PageDown;
        case GLFW_KEY_HOME:         return KeyCode::Home;
        case GLFW_KEY_END:          return KeyCode::End;
        case GLFW_KEY_CAPS_LOCK:    return KeyCode::CapsLock;
        case GLFW_KEY_SCROLL_LOCK:  return KeyCode::ScrollLock;
        case GLFW_KEY_NUM_LOCK:     return KeyCode::NumLock;
        case GLFW_KEY_PRINT_SCREEN: return KeyCode::PrintScreen;
        case GLFW_KEY_PAUSE:        return KeyCode::Pause;
        case GLFW_KEY_F1:           return KeyCode::F1;
        case GLFW_KEY_F2:           return KeyCode::F2;
        case GLFW_KEY_F3:           return KeyCode::F3;
        case GLFW_KEY_F4:           return KeyCode::F4;
        case GLFW_KEY_F5:           return KeyCode::F5;
        case GLFW_KEY_F6:           return KeyCode::F6;
        case GLFW_KEY_F7:           return KeyCode::F7;
        case GLFW_KEY_F8:           return KeyCode::F8;
        case GLFW_KEY_F9:           return KeyCode::F9;
        case GLFW_KEY_F10:          return KeyCode::F10;
        case GLFW_KEY_F11:          return KeyCode::F11;
        case GLFW_KEY_F12:          return KeyCode::F12;
        case GLFW_KEY_F13:          return KeyCode::F13;
        case GLFW_KEY_F14:          return KeyCode::F14;
        case GLFW_KEY_F15:          return KeyCode::F15;
        case GLFW_KEY_F16:          return KeyCode::F16;
        case GLFW_KEY_F17:          return KeyCode::F17;
        case GLFW_KEY_F18:          return KeyCode::F18;
        case GLFW_KEY_F19:          return KeyCode::F19;
        case GLFW_KEY_F20:          return KeyCode::F20;
        case GLFW_KEY_F21:          return KeyCode::F21;
        case GLFW_KEY_F22:          return KeyCode::F22;
        case GLFW_KEY_F23:          return KeyCode::F23;
        case GLFW_KEY_F24:          return KeyCode::F24;
        case GLFW_KEY_F25:          return KeyCode::F25;

        // Keypad keys
        case GLFW_KEY_KP_0:        return KeyCode::NumPad0;
        case GLFW_KEY_KP_1:        return KeyCode::NumPad1;
        case GLFW_KEY_KP_2:        return KeyCode::NumPad2;
        case GLFW_KEY_KP_3:        return KeyCode::NumPad3;
        case GLFW_KEY_KP_4:        return KeyCode::NumPad4;
        case GLFW_KEY_KP_5:        return KeyCode::NumPad5;
        case GLFW_KEY_KP_6:        return KeyCode::NumPad6;
        case GLFW_KEY_KP_7:        return KeyCode::NumPad7;
        case GLFW_KEY_KP_8:        return KeyCode::NumPad8;
        case GLFW_KEY_KP_9:        return KeyCode::NumPad9;
        case GLFW_KEY_KP_DECIMAL:  return KeyCode::NumPadDecimal;
        case GLFW_KEY_KP_DIVIDE:   return KeyCode::NumPadDivide;
        case GLFW_KEY_KP_MULTIPLY: return KeyCode::NumPadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return KeyCode::NumPadSubtract;
        case GLFW_KEY_KP_ADD:      return KeyCode::NumPadAdd;
        case GLFW_KEY_KP_ENTER:    return KeyCode::NumPadEnter;
        case GLFW_KEY_KP_EQUAL:    return KeyCode::NumPadEqual;

        // Modifier keys
        case GLFW_KEY_LEFT_SHIFT:    return KeyCode::LeftShift;
        case GLFW_KEY_LEFT_CONTROL:  return KeyCode::LeftControl;
        case GLFW_KEY_LEFT_ALT:      return KeyCode::LeftAlt;
        case GLFW_KEY_LEFT_SUPER:    return KeyCode::LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT:   return KeyCode::RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RightControl;
        case GLFW_KEY_RIGHT_ALT:     return KeyCode::RightAlt;
        case GLFW_KEY_RIGHT_SUPER:   return KeyCode::RightSuper;
        case GLFW_KEY_MENU:          return KeyCode::Menu;

        default: return KeyCode::Unknown;
    }
}

KeyAction ConvertGlfwAction(int glfw_action)
{
    using namespace game_engine;

    switch (glfw_action) {
        case GLFW_PRESS:   return KeyAction::Press;
        case GLFW_RELEASE: return KeyAction::Release;
        case GLFW_REPEAT:  return KeyAction::Repeat;
        default:           return KeyAction::Press;
    }
}

KeyModifier ConvertGlfwModifiers(int glfw_mods)
{
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
