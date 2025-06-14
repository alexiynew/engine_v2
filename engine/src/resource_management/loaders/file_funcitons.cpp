#include "file_funcitons.hpp"

#include <fstream>

namespace game_engine::fs
{

std::string LoadFile(const std::filesystem::path& path)
{
    namespace fs = std::filesystem;

    const auto abs_path = fs::absolute(path);

    if (!fs::exists(abs_path)) {
        throw std::runtime_error("File not exists: " + abs_path.string());
    }

    std::ifstream file(abs_path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + abs_path.string());
    }

    const auto file_size = file.tellg();
    if (file_size == -1) {
        throw std::runtime_error("Failed to read file size: " + abs_path.string());
    }

    std::string content;
    content.resize(file_size);

    if (file_size > 0) {
        file.seekg(0, std::ios::beg);
        file.read(content.data(), file_size);
        if (!file) {
            throw std::runtime_error("Failed to read file contents: " + abs_path.string());
        }
    }

    return content;
}

} // namespace game_engine::fs
