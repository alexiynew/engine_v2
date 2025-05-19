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
    int resolutionWidth       = 1600;
    int resolutionHeight      = 1200;
    std::string windowTitle   = "Game";
    int frameRate             = 60;
    int updateRate            = 60;
    DisplayMode displayMode   = DisplayMode::Windowed;
    AntiAliasing antiAliasing = AntiAliasing::None;
    bool vSync                = false;

    friend bool operator==(const GameSettings& lhs, const GameSettings& rhs) = default;
};

}; // namespace game_engine
