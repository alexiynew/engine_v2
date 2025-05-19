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

class IEngine
{
public:
    using TimePoint  = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
    using ReturnCode = int;

    virtual ~IEngine() = default;

    /// @brief Gets the current time.
    /// @return The current time point.
    virtual TimePoint GetTime() const noexcept = 0;

    /// @brief Checks if the engine should stop.
    /// @return True if the engine should stop, false otherwise.
    virtual bool ShouldStop() const noexcept = 0;

    /// @brief Creates a Mesh instance.
    /// @return New Mesh.
    virtual std::shared_ptr<graphics::IMesh> CreateMesh() = 0;

    /// @brief Creates a Shader instance.
    /// @return New Shader.
    virtual std::shared_ptr<graphics::IShader> CreateShader() = 0;

    /// @brief Renders a mesh with shader.
    /// @param mesh Mesh to render.
    /// @param shader Shader ot use to render mesh.
    /// @param uniforms Uniforms to use in shader.
    virtual void Render(const std::shared_ptr<graphics::IMesh>& mesh,
                        const std::shared_ptr<graphics::IShader>& shader,
                        const std::vector<graphics::Uniform>& uniforms) = 0;

    /// @brief Sets the flag to stop the engine.
    virtual void SetShouldStopFlag() noexcept = 0;

    [[nodiscard]]
    virtual EventSystem& GetEventSystem() const = 0;
};

} // namespace game_engine
