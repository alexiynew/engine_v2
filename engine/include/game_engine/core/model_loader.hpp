#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include <game_engine/core/mesh.hpp>

namespace game_engine::core
{

class ModelLoader
{
public:
    /// @brief Loads a model from a file.
    /// @param filePath The path to the model file.
    /// @return A shared pointer to the loaded Mesh.
    std::shared_ptr<Mesh> loadModel(const std::filesystem::path& filePath);

private:
    std::unordered_map<std::filesystem::path, std::shared_ptr<Mesh>> m_modelCache;
};

} // namespace game_engine::core
