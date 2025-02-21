#pragma once

#include <chrono>
#include <memory>

#include <game_engine/core/mesh.hpp>
#include <game_engine/core/model_loader.hpp>

namespace game_engine::core
{

// TODO: Add tests
// TODO: Add ECS
// TODO: Renderer in separate thread
// TODO: Backend in separate thread
// TODO: Game in separate thread

class Engine
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    virtual ~Engine() = default;

    /// @brief Gets the current time.
    /// @return The current time point.
    virtual TimePoint getTime() const noexcept = 0;

    /// @brief Checks if the engine should stop.
    /// @return True if the engine should stop, false otherwise.
    virtual bool shouldStop() const noexcept = 0;

    /// @brief Gets the model loader instance.
    /// @return A shared pointer to the model loader.
    virtual std::shared_ptr<ModelLoader> getModelLoader() = 0;

    /// @brief Loads a mesh to the backend.
    /// @param mesh The mesh to load.
    /// @return The ID of the loaded mesh.
    virtual MeshId loadMesh(const Mesh& mesh) = 0;

    /// @brief Renders a mesh by its ID.
    /// @param meshId The ID of the mesh to render.
    virtual void renderMesh(MeshId meshId) = 0;

    /// @brief Sets the flag to stop the engine.
    virtual void setShouldStopFlag() noexcept = 0;
};

} // namespace game_engine::core
