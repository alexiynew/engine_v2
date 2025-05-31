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

class IBackendModule
{
public:
    using RefObserver = std::reference_wrapper<IBackendObserver>;

    virtual ~IBackendModule() = default;

    static std::shared_ptr<IBackendModule> Create();

    virtual bool Init(const GameSettings& settings) noexcept = 0;
    virtual void Shutdown() noexcept                         = 0;

    /// @brief Poll events (input, window, etc.)
    virtual void PollEvents() = 0;

    virtual std::shared_ptr<const IRenderContext> GetRenderContext() const = 0;

    virtual void AttachBackendObserver(IBackendObserver& observer)       = 0;
    virtual void DetachBackendObserver(const IBackendObserver& observer) = 0;
};

} // namespace game_engine::backend
