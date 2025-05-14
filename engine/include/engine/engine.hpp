#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <engine/event_system.hpp>
#include <engine/graphics/mesh.hpp>
#include <engine/graphics/shader.hpp>

namespace game_engine
{

// TODO: Add tests for mesh loading
// TODO: Add ECS
// TODO: Game in separate thread
// TODO: Add rendering methods like in web canvas2d

class Engine
{
public:
    using TimePoint  = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
    using ReturnCode = int;

    virtual ~Engine() = default;

    /// @brief Gets the current time.
    /// @return The current time point.
    virtual TimePoint getTime() const noexcept = 0;

    /// @brief Checks if the engine should stop.
    /// @return True if the engine should stop, false otherwise.
    virtual bool shouldStop() const noexcept = 0;

    /// @brief Creates a Mesh instance.
    /// @return New Mesh.
    virtual std::shared_ptr<graphics::Mesh> createMesh() = 0;

    /// @brief Creates a Shader instance.
    /// @return New Shader.
    virtual std::shared_ptr<graphics::Shader> createShader() = 0;

    /// @brief Renders a mesh with shader.
    /// @param mesh Mesh to render.
    /// @param shader Shader ot use to render mesh.
    /// @param uniforms Uniforms to use in shader.
    virtual void render(const std::shared_ptr<graphics::Mesh>& mesh,
                        const std::shared_ptr<graphics::Shader>& shader,
                        const std::vector<graphics::Uniform>& uniforms) = 0;

    /// @brief Sets the flag to stop the engine.
    virtual void setShouldStopFlag() noexcept = 0;

    [[nodiscard]]
    virtual EventSystem& getEventSystem() const = 0;
};

} // namespace game_engine
