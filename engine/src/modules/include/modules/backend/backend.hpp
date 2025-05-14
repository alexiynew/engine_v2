#pragma once

#include <list>
#include <memory>
#include <string>

#include <engine/game_settings.hpp>
#include <engine/window_events.hpp>

#include <modules/input_handler.hpp>
#include <modules/render_context.hpp>

namespace game_engine::backend
{

class Backend
{
public:
    using RefObserver = std::reference_wrapper<BackendObserver>;

    static std::shared_ptr<Backend> Create();

    virtual ~Backend() = default;

    virtual bool init(const GameSettings& settings) noexcept = 0;
    virtual void shutdown() noexcept                         = 0;

    /// @brief Poll events (input, window, etc.)
    virtual void pollEvents() = 0;

    virtual std::shared_ptr<const RenderContext> getRenderContext() const = 0;

    virtual void attachBackendObserver(BackendObserver& observer)       = 0;
    virtual void detachBackendObserver(const BackendObserver& observer) = 0;
};

} // namespace game_engine::backend
