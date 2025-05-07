#pragma once

#include <list>
#include <memory>
#include <string>

#include <game_engine/game_settings.hpp>
#include <game_engine/system_events.hpp>

#include <modules/input_handler.hpp>
#include <modules/render_context.hpp>

namespace game_engine
{

class Backend
{
public:
    using RefObserver = std::reference_wrapper<BackendObserver>;

    virtual ~Backend() = default;

    static void RegisterModule();

    /// @brief Initialize the backend
    virtual bool initialize(const GameSettings& settings) = 0;

    /// @brief Shut down the backend
    virtual void shutdown() = 0;

    /// @brief Poll events (input, window, etc.)
    virtual void pollEvents() = 0;

    virtual std::shared_ptr<RenderContext> getRenderContext() = 0;

    virtual void attachBackendObserver(BackendObserver& observer)       = 0;
    virtual void detachBackendObserver(const BackendObserver& observer) = 0;
};

} // namespace game_engine
