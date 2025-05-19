#pragma once

#include <string>

namespace game_engine
{
enum class DisplayMode
{
    Windowed,
    Fullscreen,
    BorderlessFullscreen,
};

enum class AntiAliasing
{
    None,
    MSAA2x,
    MSAA4x,
    MSAA8x,
};

struct GameSettings
{
    int resolution_width       = 1600;
    int resolution_height      = 1200;
    std::string window_title   = "Game";
    int frame_rate             = 60;
    int update_rate            = 60;
    DisplayMode display_mode   = DisplayMode::Windowed;
    AntiAliasing anti_aliasing = AntiAliasing::None;
    bool v_sync                = false;

    friend bool operator==(const GameSettings& lhs, const GameSettings& rhs) = default;
};

}; // namespace game_engine
