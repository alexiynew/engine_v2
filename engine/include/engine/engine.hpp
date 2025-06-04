#pragma once

#include <chrono>
#include <memory>

#include <engine/events/event_system.hpp>
#include <engine/graphics/renderer.hpp>
#include <engine/resource_management/resource_manager.hpp>

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

    /// @brief Sets the flag to stop the engine.
    virtual void SetShouldStopFlag() noexcept = 0;

    [[nodiscard]]
    virtual std::shared_ptr<IResourceManager> GetResourceManager() const = 0;

    [[nodiscard]]
    virtual std::shared_ptr<IRenderer> GetRenderer() const = 0;

    [[nodiscard]]
    virtual std::shared_ptr<IEventSystem> GetEventSystem() const = 0;
};

} // namespace game_engine
