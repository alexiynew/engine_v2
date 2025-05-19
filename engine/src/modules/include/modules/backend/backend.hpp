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

class IBackend
{
public:
    using RefObserver = std::reference_wrapper<IBackendObserver>;

    static std::shared_ptr<IBackend> Create();

    virtual ~IBackend() = default;

    virtual bool Init(const GameSettings& settings) noexcept = 0;
    virtual void Shutdown() noexcept                         = 0;

    /// @brief Poll events (input, window, etc.)
    virtual void pollEvents() = 0;

    virtual std::shared_ptr<const IRenderContext> getRenderContext() const = 0;

    virtual void attachBackendObserver(IBackendObserver& observer)       = 0;
    virtual void detachBackendObserver(const IBackendObserver& observer) = 0;
};

} // namespace game_engine::backend
