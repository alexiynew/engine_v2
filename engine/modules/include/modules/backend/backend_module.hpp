#pragma once

#include <memory>

#include <engine/events/window_events.hpp>
#include <engine/game_settings.hpp>

#include <modules/input_handler.hpp>

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

    virtual void AttachBackendObserver(IBackendObserver& observer)       = 0;
    virtual void DetachBackendObserver(const IBackendObserver& observer) = 0;

    virtual void PollEvents() const = 0;

    virtual void MakeContextCurrent() const = 0;
    virtual void DropCurrentContext() const = 0;
    virtual void SwapBuffers() const        = 0;
};

} // namespace game_engine::backend
