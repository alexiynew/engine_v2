#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <game_engine/core/mesh.hpp>
#include <game_engine/core/model_loader.hpp>
#include <game_engine/core/shader.hpp>

namespace game_engine::core
{

// TODO: Add tests for mesh loading
// TODO: Add ECS
// TODO: Renderer in separate thread
// TODO: Backend in separate thread
// TODO: Game in separate thread
// TODO: Add rendering methods like in web canvas2d

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

    /// @brief Creates a Mesh instance.
    /// @return New Mesh.
    virtual std::shared_ptr<Mesh> createMesh() = 0;

    /// @brief Creates a Shader instance.
    /// @return New Shader.
    virtual std::shared_ptr<Shader> createShader() = 0;

    /// @brief Renders a mesh with shader.
    /// @param mesh Mesh to render.
    /// @param shader Shader ot use to render mesh.
    /// @param unifors Unifors to use in shader.
    virtual void render(const std::shared_ptr<Mesh>& mesh,
                        const std::shared_ptr<Shader>& shader,
                        const std::vector<Uniform>& uniforms) = 0;

    /// @brief Sets the flag to stop the engine.
    virtual void setShouldStopFlag() noexcept = 0;
};

} // namespace game_engine::core
