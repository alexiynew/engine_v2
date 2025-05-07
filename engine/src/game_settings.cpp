#include <game_engine/game_settings.hpp>

namespace game_engine
{
bool operator==(const GameSettings& lhs, const GameSettings& rhs)
{
    return lhs.resolutionWidth == rhs.resolutionWidth && lhs.resolutionHeight == rhs.resolutionHeight &&
           lhs.windowTitle == rhs.windowTitle && lhs.frameRate == rhs.frameRate && lhs.updateRate == rhs.updateRate &&
           lhs.displayMode == rhs.displayMode && lhs.antiAliasing == rhs.antiAliasing && lhs.vSync == rhs.vSync;
}

bool operator!=(const GameSettings& lhs, const GameSettings& rhs)
{
    return !(lhs == rhs);
}

} // namespace game_engine
