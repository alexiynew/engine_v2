#pragma once

#include <chrono>
#include <memory>

namespace game_engine::core
{

class Engine
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    virtual ~Engine() = default;

    virtual TimePoint get_time() const noexcept = 0;
    virtual bool should_stop() const noexcept   = 0;

    virtual void set_should_stop_flag() noexcept = 0;
};

} // namespace game_engine::core
