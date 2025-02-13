#pragma once

namespace game_engine
{
enum class KeyCode
{
    Unknown = -1,
    Space   = 32,
    Escape  = 256,
    // Add more keys
};

enum class KeyAction
{
    None,
    Press,
    Release,
    Repeat,
};

enum class KeyModifier
{
    None    = 0,
    Shift   = 1,
    Control = 2,
    Alt     = 4,
    Super   = 8,
};

struct KeyboardInputEvent
{
    KeyCode key           = KeyCode::Unknown;
    KeyAction action      = KeyAction::None;
    KeyModifier modifiers = KeyModifier::None;
};

} // namespace game_engine
