#pragma once

#include <filesystem>
#include <string>

namespace game_engine::fs
{

std::string LoadFile(const std::filesystem::path& path);

} // namespace game_engine::fs
